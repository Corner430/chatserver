cmake_minimum_required(VERSION 3.10) # 指定cmake的最低版本
project(chat)   # 指定项目名称

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/MySQLConnectionPool)
include_directories(${PROJECT_SOURCE_DIR}/include/server/dbORM)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
# link_directories(/usr/lib64/mysql)

# 设置动态链接库文件目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 加载子目录
add_subdirectory(src)