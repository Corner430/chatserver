#pragma once

#include <mysql/mysql.h>
#include <string>

using namespace std;

class Connection {
public:
  Connection();  // 初始化数据库连接
  ~Connection(); // 释放数据库连接资源
  bool connect(string ip, unsigned int port, string username, string password,
               string dbname);  // 连接数据库
  bool update(string sql);      // 更新操作, insert, update, delete
  MYSQL_RES *query(string sql); // 查询操作, select

  // 获取当前 MySQL 连接
  MYSQL *getConnection() const { return _conn; }

  // 刷新连接的起始的空闲时间点，clock() 返回的是 CPU 时间
  void refreshAliveTime() { _alivetime = clock(); }

  // 返回存活的时间
  clock_t getAliveTime() const { return clock() - _alivetime; }

private:
  MYSQL *_conn;       // 数据库连接
  clock_t _alivetime; // 记录连接的空闲时间起始点
};
