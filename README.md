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

## 4 数据库设计

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
