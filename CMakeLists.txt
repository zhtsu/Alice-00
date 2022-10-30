cmake_minimum_required(VERSION 3.16.0)

project(Alice)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set(CMAKE_CXX_STANDARD 17)

# 设置 spdlog 的参数
set(SPDLOG_BUILD_SHARED ON)
# 设置 GLFT 的参数
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(BUILD_SHARED_LIBS ON)

include_directories(${PROJECT_SOURCE_DIR}/src/)

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glfw-3.3.3)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/imgui-1.67)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/spdlog-1.10.0)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice/Core)

add_executable(
    Alice
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
)

target_precompile_headers(Alice PUBLIC ${PROJECT_SOURCE_DIR}/src/Alice/Core/PCH.hpp)

target_link_libraries(
    Alice
    Alice_UI
)
