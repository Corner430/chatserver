#pragma once

/*
 * 数据库连接池
 */

#include "Connection.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
using namespace std;

class ConnectionPool {
public:
  static ConnectionPool *getConnectionPool(); // 获取连接池对象
  shared_ptr<Connection> getConnection();     // 获取空闲连接

private:
  ConnectionPool(); // 单例模式

  bool loadConfigFile(); // 从配置文件中加载配置

  void produceConnectionTask(); // 生产者线程，生产新连接

  void scannerConnectionTask(); // 定时检测空闲连接，回收超时连接

  string _ip;             // 数据库ip
  unsigned short _port;   // 数据库端口
  string _username;       // 数据库用户名
  string _password;       // 数据库密码
  string _dbname;         // 数据库名
  int _initSize;          // 数据库连接池连接初始数量
  int _maxSize;           // 数据库连接池最大数量
  int _maxIdleTime;       // 最大空闲时间
  int _connectionTimeout; // 数据库连接超时时间

  queue<Connection *> _connectionQue; // 连接队列
  mutex _queueMutex;                  // 连接队列互斥锁
  condition_variable cv;     // 条件变量，用于唤醒消费者线程
  atomic_int _connectionCnt; // 当前已创建的连接数
};