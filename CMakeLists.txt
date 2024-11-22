cmake_minimum_required(VERSION 3.15)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件
add_executable(main main.cpp)

# 链接依赖的库（例如 IPHelper 和 Ws2_32）
target_link_libraries(main iphlpapi ws2_32)
