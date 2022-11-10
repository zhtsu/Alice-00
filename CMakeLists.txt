cmake_minimum_required(VERSION 3.16.0)

project(Alice)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set(CMAKE_CXX_STANDARD 17)

# 设置 spdlog 的参数
set(SPDLOG_BUILD_SHARED ON)
# 设置 GLFW 的参数
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(BUILD_SHARED_LIBS ON)

include_directories(${PROJECT_SOURCE_DIR}/src/)

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/imgui)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/spdlog)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/stb)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice)

add_executable(
    Alice
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
)

target_precompile_headers(Alice PUBLIC ${PROJECT_SOURCE_DIR}/src/Alice/PCH.hpp)

target_link_libraries(
    Alice
    Alice_UI
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"    
)
