#ifndef REDIS_H
#define REDIS_H

#include <functional>
#include <hiredis/hiredis.h>
#include <thread>
using namespace std;

/*
 * redis 作为集群服务器通信的基于发布-订阅消息队列时，会遇到两个难搞的 bug 问题
 * bug 1. 向消息中间件 PUBLISH 发布消息不成功，导致无法跨服务器通信
 * bug 2. 多次向中间件 SUBSCRIBE 订阅消息，出现无响应，客户端也无响应
 */
class Redis {
public:
  Redis();
  ~Redis();

  // 连接 redis 服务器
  bool connect();

  // 向 redis 指定的通道 channel 发布消息
  bool publish(int channel, string message);

  // 向 redis 指定的通道 subscribe 订阅消息
  bool subscribe(int channel);

  // 向 redis 指定的通道 unsubscribe 取消订阅消息
  bool unsubscribe(int channel);

  // 在独立线程中接收订阅通道中的消息
  void observer_channel_message();

  // 初始化向业务层上报通道消息的回调对象
  void init_notify_handler(function<void(int, string)> fn);

private:
  // hiredis 同步上下文对象，负责 publish 消息
  redisContext *_publish_context;

  // hiredis 同步上下文对象，负责 subscribe 消息
  redisContext *_subcribe_context;

  // 回调操作，收到订阅的消息，给service层上报
  function<void(int, string)> _notify_message_handler;
};

#endif