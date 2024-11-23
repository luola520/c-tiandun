cmake_minimum_required(VERSION 3.10)

# 设置项目名称和 C++ 标准
project(MyApp VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加 include 文件夹到编译器的 include 路径
include_directories(include)

# 查找 curl 库
find_package(CURL REQUIRED)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
endif()

# 添加可执行文件
add_executable(MyApp src/main.cpp)

# 链接所需的库
target_link_libraries(MyApp PRIVATE ${CURL_LIBRARIES})
if(WIN32)
    target_link_libraries(MyApp PRIVATE iphlpapi)
endif()

# 定义输出目录
set_target_properties(MyApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
