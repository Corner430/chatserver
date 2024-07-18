#include "friendmodel.hpp"

// 添加好友关系
void FriendModel::insert(int userid, int friendid) {
  // 1. 组装 sql 语句
  char sql[1024] = {0};
  sprintf(sql, "insert into friend values(%d, %d)", userid, friendid);

  shared_ptr<Connection> sp = _connPool->getConnection();
  if(sp) sp->update(sql);
}

// 返回用户好友列表
vector<User> FriendModel::query(int userid) {
  // 1. 组装 sql 语句
  char sql[1024] = {0};

  sprintf(sql,
          "select a.id, a.name, a.state from user a inner join friend b on "
          "b.friendid = a.id where b.userid=%d",
          userid);

  vector<User> vec;
  shared_ptr<Connection> sp = _connPool->getConnection();
  if (sp) {
    MYSQL_RES *res = sp->query(sql);
    if (res != nullptr) {
      // 把 userid 用户的所有好友信息放入 vec 中返回
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res)) != nullptr) {
        User user;
        user.setId(atoi(row[0]));
        user.setName(row[1]);
        user.setState(row[2]);
        vec.push_back(user);
      }
      mysql_free_result(res);
      return vec;
    }
  }
  return vec;
}