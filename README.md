# 集群聊天服务器单机版

## 1 技术栈

- Json 序列化和反序列化
- muduo 网络库开发
- MySQL 连接池的使用

## 2 项目需求

1. 客户端新用户注册
2. 客户端用户登录
3. 客户端用户退出
4. 添加好友和群组
5. 好友聊天
6. 群组聊天
7. 离线消息

## 3 项目目标

1. 掌握服务器的网络 I/O 模块，业务模块，数据模块分层的设计思想
2. 掌握 C++ muduo 网络库的编程以及实现原理
3. 掌握 Json 的编程应用

## 4 开发环境

1. 操作系统：Ubuntu 22.04
2. [json for Modern C++](https://github.com/nlohmann/json)
3. [muduo](https://github.com/chenshuo/muduo)

## 5 数据库设计

**User 表**

| 字段名 | 类型 | 说明 | 约束 ｜
｜ --- | --- | --- | --- |
｜ id | INT | 用户 id | PRIMARY KEY, AUTO_INCREMENT |
| name | VARCHAR(50) | 用户名 | NOT NULL, UNIQUE |
| password | VARCHAR(50) | 密码 | NOT NULL |
| state | ENUM('online', 'offline') | 用户登录状态 | DEFAULT 'offline' |

**Friend 表**

| 字段名 | 类型 | 说明 | 约束 ｜
｜ --- | --- | --- | --- |
｜ userid | INT | 用户 id | NOT NULL , 联合主键 |
| friendid | INT | 好友 id | NOT NULL, 联合主键 |

**AllGroup 表**

| 字段名 | 类型 | 说明 | 约束 ｜
｜ --- | --- | --- | --- |
| id | INT | 群组 id | PRIMARY KEY, AUTO_INCREMENT |
| groupname | VARCHAR(50) | 群组名 | NOT NULL, UNIQUE |
| groupdesc | VARCHAR(200) | 群组描述 | DEFAULT '' |

**GroupUser 表**

| 字段名 | 类型 | 说明 | 约束 ｜
｜ --- | --- | --- | --- |
| groupid | INT | 群组 id | NOT NULL , 联合主键 |
| userid | INT | 组员 id | NOT NULL, 联合主键 |
| grouprole | ENUM('creator', 'normal') | 组员角色 | DEFAULT 'normal' |

**OfflineMessage 表**

| 字段名 | 类型 | 说明 | 约束 ｜
｜ --- | --- | --- | --- |
| userid | INT | 用户 id | NOT NULL |
| message | VARCHAR(500) | 离线消息（存储 Json 字符串） | NOT NULL |