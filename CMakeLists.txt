cmake_minimum_required(VERSION 3.23.0)

project(Alice)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

# 设置 spdlog 的参数
set(SPDLOG_BUILD_SHARED ON)
# 设置 GLFT 的参数
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(BUILD_SHARED_LIBS ON)

# 设置 spdlog 远程下载地址
set(
    3RD_PARTY_SPDLOG
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.10.0.zip
)

# 导入下载功能
include(FetchContent)

# 下载 spdlog
message(">> Downloading spdlog from ${3RD_PARTY_SPDLOG}...")
FetchContent_Declare(
    spdlog
    ${3RD_PARTY_SPDLOG}
)
FetchContent_MakeAvailable(spdlog)
message(">> Successfully download spdlog!")

include_directories(${PROJECT_SOURCE_DIR}/src/)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glfw-3.3.3)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glad)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice/Application)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice/Log)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice/Events)

add_executable(
    Alice
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
)

target_precompile_headers(Alice PUBLIC ${PROJECT_SOURCE_DIR}/src/Alice/PCH.hpp)

target_link_libraries(Alice Alice_Application)
