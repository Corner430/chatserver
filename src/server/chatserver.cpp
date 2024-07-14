#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"

#include <functional>
#include <iostream>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop, const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop) {
  // 注册连接回调
  _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

  // 注册消息回调
  _server.setMessageCallback(
      std::bind(&ChatServer::onMessage, this, _1, _2, _3));

  // 设置线程数量
  _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start() { _server.start(); }

// 上报连接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn) {
  if (!conn->connected()) { // 客户端断开链接
    ChatService::instance()->clientCloseException(conn);
    conn->shutdown();
  }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer,
                           Timestamp time) {
  string buf = buffer->retrieveAllAsString();

  // 测试，添加json打印代码
  // cout << buf << endl;

  // 数据的反序列化
  json js = json::parse(buf);

  // 作用：完全解耦网络模块的代码和业务模块的代码
  // 通过 js["msgid"] 的 get<int>() 方法获取消息类型
  // 根据消息类型，返回对应的业务处理器（就是一个回调（函数对象））
  auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());

  // 通过事先绑定好的回调函数，来执行相应的业务处理
  msgHandler(conn, js, time);
}