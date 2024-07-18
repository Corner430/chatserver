#pragma once

#include "CommonConnectionPool.h"

class model {
public:
  model() { _connPool = ConnectionPool::getConnectionPool(); }
  // 单例对象的连接池
  ConnectionPool *_connPool;
};