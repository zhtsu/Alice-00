cmake_minimum_required(VERSION 3.16.0)

project(Alice)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS ON)

# set spdlog
set(SPDLOG_BUILD_SHARED ON)
# set GLFW
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

include_directories(${PROJECT_SOURCE_DIR}/src/)

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/imgui)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/spdlog)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/stb_image)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/entt)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/yaml-cpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/nativefiledialog)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/ImGuizmo)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/box2d)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Alice)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Sandbox)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Editor)

add_executable(
    Alice
    ${PROJECT_SOURCE_DIR}/src/Main.cpp
)

target_precompile_headers(Alice PUBLIC ${PROJECT_SOURCE_DIR}/src/Alice/PCH.hpp)

target_link_libraries(
    Alice
    Editor
    Sandbox
)

# @TODO:
# Remove old assets before copy current assets
# add_custom_command(
#     TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E remove_directory
#     "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"  
# )

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"    
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources"    
)
