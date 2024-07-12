# 配置头文件搜索路径
# include_directories()

# 配置库文件搜索路径
# link_directories()

# 设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 把 . 路径下的所有源文件的名字保存在变量 SRC_LIST 中
# aux_source_directory(. SRC_LIST)

# 由 SRC_LIST 生成可执行文件 testmuduo
add_executable(testmuduo ${SRC_LIST})

# 链接库文件
target_link_libraries(testmuduo muduo_net muduo_base pthread)