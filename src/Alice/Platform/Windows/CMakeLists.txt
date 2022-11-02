cmake_minimum_required(VERSION 3.16.0)

add_compile_definitions(GLFW_INCLUDE_NONE)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Platform_Windows
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Platform_Windows
    Alice_Application
    Alice_Log
    glfw
    glad
    Alice_Platform_OpenGL
)

set_target_properties(Alice_Platform_Windows PROPERTIES OUTPUT_NAME "alice-platform-windows")
