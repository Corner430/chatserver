/*
muduo 网络库给用户提供了两个主要的累
TcpServer ： 用于编写服务器程序
TcpClient ： 用于编写客户端程序

epoll + 线程池 + 事件循环 + 事件驱动
优点：可以把网络 I/O 和业务处理分开编写
*/

#include <iostream>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

/* 基于 muduo 网络库开发服务器程序的主要步骤
1. 创建一个 EventLoop 对象 loop
2. 创建一个 InetAddress 对象 listenAddr，指定服务器的 IP 地址和端口
3. 创建一个 TcpServer 对象 server
4. 设置 TcpServer 对象 server 的各种属性
5. 设置 TcpServer 对象 server 的各种回调函数
6. 调用 TcpServer 对象 server 的 start() 函数
7. 在 EventLoop 对象 loop 的 loop() 函数中循环等待客户端的连接和消息
*/

class ChatServer {
public:
  ChatServer(EventLoop *loop, const InetAddress &listenAddr,
             const string &nameArg)
      : server_(loop, listenAddr, nameArg), loop_(loop) {
    // 注册连接回调函数（客户端连接和断开）
    server_.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));

    // 注册消息回调函数（读写事件）
    server_.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量 4 个（1 个 I/O 线程，3 个工作线程）
    server_.setThreadNum(4);
  }

  void start() { server_.start(); }

private:
  // 连接回调函数
  void onConnection(const TcpConnectionPtr &conn) {
    if (conn->connected())
      cout << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " state: connected\n";
    else {
      cout << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " state: disconnected\n";
      conn->shutdown(); // close(fd)
    }
  }

  // 消息回调函数，处理读写事件
  void onMessage(const TcpConnectionPtr &conn /* 连接 */,
                 Buffer *buffer /* 缓冲区 */,
                 Timestamp time /* 接收到数据的时间信息 */) {
    string buf = buffer->retrieveAllAsString();
    cout << "recv data: " << buf << " time: " << time.toFormattedString()
         << endl;
    conn->send(buf);
  }

  TcpServer server_; // 服务器对象
  EventLoop *loop_;  // 事件循环对象（epoll）
};

int main() {
  EventLoop loop;
  InetAddress listenAddr(8888);
  // InetAddress listenAddr("127.0.0.1", 8888);
  ChatServer server(&loop, listenAddr, "ChatServer");

  server.start(); // 启动服务器，epoll_ctl 读事件
  loop.loop();    // 进入事件循环，epoll_wait
               // 阻塞等待事件（新用户的连接、已连接用户的读写事件）发生
  return 0;
}