# 使用 Ubuntu 22.04 镜像作为基础镜像
FROM ubuntu:22.04

# 定义作者信息
LABEL author="corner" version="1.0"

# 工作目录
WORKDIR /root

# 安装所需软件包
RUN apt update && \
   apt upgrade -y && \
   DEBIAN_FRONTEND=noninteractive apt-get install -y \
   vim \
   build-essential \
   g++ \
   gdb \
   cmake \
   make \
   openssh-server \
   git \
   libboost-dev \
   mysql-server \
   libmysqlclient-dev


# ssh，允许 root 密码登录，并设置密码为 1
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
   sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
   echo 'root:1' | chpasswd

# 拷贝 ssh 启动脚本
COPY start_ssh.sh /root/start_ssh.sh

# 增加可执行权限
RUN chmod +x /root/start_ssh.sh

# 写追加到 .bashrc 文件
ADD bashrc /root/bashrc  
RUN echo "$(cat /root/bashrc)" >> /root/.bashrc
RUN rm -f /root/bashrc

# 创建 /root/project 文件夹
RUN mkdir /root/chatserver

# 下载 json.hpp 到 /usr/include
# RUN wget https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp -O /usr/include/json.hpp

# 下载 muduo 到 /root 目录
RUN git clone https://github.com/chenshuo/muduo.git /root/muduo

# 注释掉 muduo 的 examples
RUN sed -i 's/^\s*option(MUDUO_BUILD_EXAMPLES/# &/' /root/muduo/CMakeLists.txt

# 进入 muduo 目录并编译
WORKDIR /root/muduo
RUN $PWD/build.sh
RUN $PWD/build.sh install

# 安装 muduo 到系统目录
RUN mv /root/build/release-install-cpp11/include/muduo /usr/include/muduo
RUN mv /root/build/release-install-cpp11/lib/libmuduo* /usr/local/lib

# 删除 muduo 目录
RUN rm -rf /root/muduo
RUN rm -rf /root/build

# 默认启动 /bin/bash
CMD ["/bin/bash"]
