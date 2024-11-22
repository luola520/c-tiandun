cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)

# 添加 curl 头文件和库文件的路径
include_directories(C:/ProgramData/chocolatey/lib/curl/tools/curl-7.87.0-win64-mingw/include)
link_directories(C:/ProgramData/chocolatey/lib/curl/tools/curl-7.87.0-win64-mingw/lib)

# 链接 curl 库
link_libraries(curl)

# 添加可执行文件
add_executable(your_executable_name main.cpp)
