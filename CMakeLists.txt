cmake_minimum_required(VERSION 3.15)
project(WindowsBuildExample)

set(CMAKE_CXX_STANDARD 17)

# 添加 JSON 库路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源代码
add_executable(main main.cpp)

# 链接库
target_link_libraries(main ws2_32 iphlpapi)
