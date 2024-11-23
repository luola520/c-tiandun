cmake_minimum_required(VERSION 3.10)

# 项目设置
project(MyApp VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加 include 文件夹
include_directories(include)

# 添加 libcurl 依赖路径（调整为你本地的 cURL 安装路径）
if(WIN32)
    set(CURL_INCLUDE_DIR "C:/path/to/curl/include")  # 修改为实际路径
    set(CURL_LIBRARIES "C:/path/to/curl/lib/libcurl.lib")  # 修改为实际路径
    include_directories(${CURL_INCLUDE_DIR})
endif()

# 查找并链接 cURL
find_package(CURL REQUIRED)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    message(STATUS "cURL found: ${CURL_LIBRARIES}")
else()
    message(FATAL_ERROR "cURL not found!")
endif()

# 添加源代码
add_executable(MyApp src/main.cpp)

# 链接库
target_link_libraries(MyApp PRIVATE ${CURL_LIBRARIES} iphlpapi)

# 输出目录
set_target_properties(MyApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
