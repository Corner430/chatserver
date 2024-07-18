# 定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./model MODEL_LIST)

# 定义 MySQL 配置文件路径
add_definitions(-DMYSQL_CONFIG_FILE_PATH=\"${PROJECT_SOURCE_DIR}/config/mysql.conf\")

# 指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${MODEL_LIST})

# 指定可执行文件链接时需要依赖的库文件
target_link_libraries(ChatServer muduo_net muduo_base mysqlclient mysql_connection_pool pthread)