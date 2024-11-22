cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)

include_directories(include) # 包含 nlohmann/json.hpp 文件的目录
link_libraries(curl iphlpapi) # 链接 curl 和 iphlpapi 库

add_executable(your_executable_name main.cpp) # 替换 main.cpp 为你的主源文件路径
