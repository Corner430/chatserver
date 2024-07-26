# 开发日志

### 1 test json

### 2 test muduo

### 3 目录结构 + CMakeLists.txt

```shell
.
├── bin
│   ├── ChatClient
│   └── ChatServer
├── CMakeLists.txt
├── include
│   └── server
├── lib
├── logs.md
├── README.md
├── src
│   ├── client
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── server
│       └── CMakeLists.txt
├── test
│   ├── testjson
│   │   ├── json.hpp
│   │   └── testjson.cpp
│   └── testmuduo
│       ├── CMakeLists.txt
│       └── muduo_server.cpp
└── thirdparty
    └── json.hpp
```

### 4 项目数据库以及表的设计

### 5 网络模块 ChatServer

```shell
/include/server/ChatServer.hpp
/src/server/ChatServer.cpp
```

### 6 业务模块 ChatService

```shell
/include/server/ChatService.hpp
/src/server/ChatService.cpp
/include/public.hpp
```

1. `static` 实现单例模式
2. 通过 `MsgHandler getHandler(int msgid);` 和预置回调**解耦网络模块和业务模块**
3. 使用 `unordered_map` 存储消息号和回调函数的映射关系，并使用绑定器 `std::bind` 进行绑定

> **对网络模块分发业务事件回调操作功能进行测试**

### 7 数据库模块

```shell
/include/server/MySQLConnectionPoll/
```

参照 [MySQL Connection Pool](https://github.com/Corner430/MySQL-Connection-Pool) 使用数据库连接池

~~在 `ChatService` 中添加数据库连接池对象，并在构造函数中初始化~~

### 8 Model 数据层代码框架设计

```shell
/include/server/dbORM/  # 数据库 ORM 框架
/include/server/model/
/src/server/model/
```

1. `dbORM` 数据库 ORM 框架
2. `model` 数据层代码框架设计
3. **`include/server/model/model.hpp` 中定义了单例模式的连接池对象，被所有的 Model 类继承共享**

> **解耦业务模块和数据库模块**

### 9 用户登陆/注册/退出业务实现及测试

1. `ChatService::login`, `ChatService::reg`, `ChatService::loginout`, `usermodel`
2. 使用 `unordered_map<int, TcpConnectionPtr> _userConnMap;` 存储在线用户的通信连接，为保证线程安全，使用 `lock_guard` 进行加锁

### 10 客户端异常退出业务实现及测试

1. `ChatService::clientCloseException()` 方法，当客户端异常退出时，将用户从在线用户列表中删除，并更新数据库中用户的状态
2. 修改 `ChatServer::onConnection()` 方法，当客户端异常退出时，调用 `ChatService::clientCloseException` 方法

### 11 点对点聊天业务实现及测试

`ChatService::oneChat()` 方法，实现点对点聊天业务

### 12 离线消息业务实现及测试

offlinemessagemodel.cpp

### 13 服务器异常退出实现及测试

`/src/server/main.cpp` 中添加 [`signal`](https://en.cppreference.com/w/cpp/utility/program/signal) 信号处理函数，当服务器异常退出时，调用 `ChatService::reset()` 方法，将在线用户从数据库中更新为离线状态

### 14 添加好友业务实现及测试

`ChatService::addFriend()`, `friendmodel.cpp`

### 15 群组聊天相关业务

### 16 客户端开发

> 对于当前用户的信息，好友信息，群组信息进行存储，避免反复从服务器获取

#### 16.1 首页界面功能开发及测试

#### 16.2 添加好友和聊天功能开发及测试

#### 16.3 离线消息功能开发及测试

#### 16.4 群组聊天功能开发及测试

#### 16.5 用户注销功能开发及测试

#### 17 基于 redis 发布-订阅的消息队列实现集群服务器间实时通信

#### 18 nginx 反向代理实现负载均衡