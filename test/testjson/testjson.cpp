#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <string>
using namespace std;

// json 序列化示例 1
string func1() {
  json js;
  js["msg_type"] = 2;
  js["from"] = "zhang san";
  js["to"] = "li si";
  js["msg"] = "hello, what are you doing now?";

  string sendBuf = js.dump(); // 序列化
  // cout << "序列化后的json数据: " << sendBuf << endl;
  return sendBuf;
}

// json 序列化示例 2
string func2() {
  json js;
  // 添加数组
  js["id"] = {1, 2, 3, 4, 5};
  // 添加 key-value
  js["name"] = "zhang san";
  // 添加对象
  js["msg"]["zhang san"] = "hello python";
  js["msg"]["liu shuo"] = "hello c++";
  // 上面等同于下面这句一次性添加数组对象
  js["msg"] = {{"zhang san", "hello python"}, {"liu shuo", "hello c++"}};
  // cout << js << endl;
  // cout << "序列化后的json数据: " << js.dump() << endl;
  return js.dump();
}

// json 反序列化示例 3
string func3() {
  json js;

  // 直接序列化一个 vector 容器
  vector<int> vec = {1, 2, 5};
  js["list"] = vec;

  // 直接序列化一个 map 容器
  map<int, string> m;
  m[1] = "zhang san";
  m[2] = "li si";
  js["name"] = m;

  string sendBuf = js.dump(); // 序列化
  // cout << "序列化后的json数据: " << sendBuf << endl;
  return sendBuf;
}

int main() {
  string recvBuf = func1();
  json jsbuf = json::parse(recvBuf); // 反序列化
  cout << jsbuf["msg_type"] << endl;
  cout << jsbuf["from"] << endl;
  cout << jsbuf["to"] << endl;
  cout << jsbuf["msg"] << endl;

  cout << "-----------------" << endl;

  recvBuf = func2();
  jsbuf = json::parse(recvBuf); // 反序列化
  cout << jsbuf["id"] << endl;
  auto arr = jsbuf["id"];
  cout << arr[2] << endl;
  auto msgjs = jsbuf["msg"];
  cout << msgjs["zhang san"] << endl;
  cout << msgjs["liu shuo"] << endl;

  cout << "-----------------" << endl;

  recvBuf = func3();
  jsbuf = json::parse(recvBuf); // 反序列化
  vector<int> vec = jsbuf["list"];
  // js 对象里面的数组类型，直接放入 vector 容器中
  for (int &v : vec)
    cout << v << " ";
  cout << endl;

  map<int, string> mymap = jsbuf["name"];
  for (auto &m : mymap)
    cout << m.first << " " << m.second << endl;
  cout << endl;

  return 0;
}