# 聊天服务器集群版

使用方式

```shell
mkdir build && cmake .. && make # 之后运行可执行文件即可
```

## 1 开发环境

[集群版 Docker 开发环境：Ubuntu 22.04](https://github.com/Corner430/Docker/tree/main/chatserver-cluster)

用到的技术栈：

- Json 序列化和反序列化，借助于 [json for Modern C++](https://github.com/nlohmann/json)
- muduo 网络库开发，借助于 [muduo](https://github.com/chenshuo/muduo)
- [nginx 源码](https://github.com/nginx/nginx)编译安装和环境部署
- nginx 的 tcp 负载均衡器配置
- 基于发布-订阅的服务器中间件 redis 消息队列编程实践，借助于 [hiredis](https://github.com/redis/hiredis)
- MySQL 连接池的使用，借助于 [MySQL-Connection-Pool](https://github.com/Corner430/MySQL-Connection-Pool)


## 2 项目需求

1. 客户端新用户注册
2. 客户端用户登录
3. 客户端用户退出
4. 添加好友和群组
5. 好友聊天
6. 群组聊天
7. 离线消息
8. nginx 配置 tcp 负载均衡
9. 集群聊天系统支持客户端跨服务器通信

## 3 项目目标

1. 掌握服务器的网络 I/O 模块，业务模块，数据模块分层的设计思想
2. 掌握 C++ muduo 网络库的编程以及实现原理
3. 掌握 Json 的编程应用
4. 掌握 MySQL 连接池的使用
5. 掌握 Docker 一键部署
6. 掌握 CMake
7. 掌握 nginx 配置部署 tcp 负载均衡器的应用以及原理
8. 掌握服务器中间件的应用场景和基于发布-订阅的 redis 编程实践以及应用原理

## 4 服务器中间件-基于发布-订阅的 Redis

### 4.1 集群服务器之间的通信设计

当 ChatServer 集群部署多台服务器以后，登录在不同服务器上的用户如何进行通信？

![20240719114554](https://cdn.jsdelivr.net/gh/Corner430/Picture/images/20240719114554.png)

上面的设计，让各个 ChatServer 服务器**两两之间**建立 TCP 连接进行通信，这等价于在服务器网络之间进行广播。这样的设计使用每个服务器即是服务端，又是客户端，各个服务器之间耦合度太高，不利于系统扩展，并且会占用系统大量的 socket 资源，各服务器之间的带宽压力很大，不能够节省资源给更多的客户端提供服务，因此这是一个很不好的设计！

集群部署的服务器之间进行通信，**最好的方式就是引入中间件消息队列**，解耦各个服务器，使整个系统松耦合，提高服务器的响应能力，节省服务器的带宽资源，如下图所示：

![20240719114641](https://cdn.jsdelivr.net/gh/Corner430/Picture/images/20240719114641.png)

### 4.2 Redis 发布-订阅相关命令

redis 的发布-订阅机制：发布-订阅模式包含了两种角色，分别是消息的发布者和消息的订阅者。订阅者可以订阅一个或者多个 channel，发布者可以向指定的 channel 发送消息，所有订阅了该 channel 的订阅者都会收到消息。

订阅 channel：`SUBSCRIBE channel [channel ...]`

```shell
root@14178aa96595:~# redis-cli
127.0.0.1:6379> SUBSCRIBE channel "corner"
Reading messages... (press Ctrl-C to quit)
1) "subscribe"
2) "channel"
3) (integer) 1
1) "subscribe"
2) "corner"
3) (integer) 2
1) "message"
2) "corner"
3) "hello corner!"
```

> 说明：`SUBSCRIBE` 命令会阻塞当前客户端，直到有消息发布到指定的 channel，这时候会收到消息。此状态下，只能执行 `subscribe`、`unsubscribe`、`psubscribe`、`punsubscribe` 四个命令。

另一个客户端：

```shell
root@14178aa96595:~# redis-cli
127.0.0.1:6379> publish "corner" "hello corner!"
(integer) 1
127.0.0.1:6379> 
```

> 说明：`publish` 命令用于向指定的 channel 发送消息，返回值是订阅该 channel 的订阅者数量。

进入订阅状态后客户端可能收到 3 种类型的回复。每种类型的回复都包含 3 个值，分别是消息类型、channel 名称和消息体。根据消息类型的不同，后两个值的意义也不同。消息类型的取值如下：
1. `subscribe`：订阅成功。第二个值是订阅成功的 channel 名称，第三个值是当前客户端订阅的 channel 数量。
2. `message`：收到消息。第二个值是产生消息的 channel 名称，第三个值是消息体。
3. `unsubscribe`：取消订阅。第二个值是取消订阅的 channel 名称，第三个值是当前客户端订阅的 channel 数量，如果数量为 0，则客户端会退出订阅状态。

### 4.3 Redis 发布-订阅的客户端编程

redis 支持不同的客户端编程语言，如 C、C++、Python、Java 等。这里使用 C++ 编程语言，使用 [hiredis](https://github.com/redis/hiredis) 库进行编程。

安装 hiredis 库：

```shell
git clone https://github.com/redis/hiredis  # 下载 hiredis 源码
cd hiredis  # 进入 hiredis 目录
make    # 编译
make install  # 安装，将头文件拷贝到 /usr/local/include，将库文件拷贝到 /usr/local/lib
ldconfig /usr/local/lib   # 更新动态链接库
```

## 5 数据库设计

**User 表**

| 字段名 | 类型 | 说明 | 约束 |
| --- | --- | --- | --- |
| id | INT | 用户 id | PRIMARY KEY , AUTO_INCREMENT |
| name | VARCHAR(50) | 用户名 | NOT NULL , UNIQUE |
| password | VARCHAR(50) | 密码 | NOT NULL |
| state | ENUM('online', 'offline') | 用户登录状态 | DEFAULT 'offline' |

**Friend 表**

| 字段名 | 类型 | 说明 | 约束 |
| --- | --- | --- | --- |
| userid | INT | 用户 id | NOT NULL , 联合主键 |
| friendid | INT | 好友 id | NOT NULL , 联合主键 |

**AllGroup 表**

| 字段名 | 类型 | 说明 | 约束 |
| --- | --- | --- | --- |
| id | INT | 群组 id | PRIMARY KEY , AUTO_INCREMENT |
| groupname | VARCHAR(50) | 群组名 | NOT NULL , UNIQUE |
| groupdesc | VARCHAR(200) | 群组描述 | DEFAULT '' |

**GroupUser 表**

| 字段名 | 类型 | 说明 | 约束 |
| --- | --- | --- | --- |
| groupid | INT | 群组 id | NOT NULL , 联合主键 |
| userid | INT | 组员 id | NOT NULL, 联合主键 |
| grouprole | ENUM('creator', 'normal') | 组员角色 | DEFAULT 'normal' |

**OfflineMessage 表**

| 字段名 | 类型 | 说明 | 约束 |
| --- | --- | --- | --- |
| userid | INT | 用户 id | NOT NULL |
| message | VARCHAR(500) | 离线消息（存储 Json 字符串） | NOT NULL |
