cmake_minimum_required(VERSION 3.16.0)

add_compile_definitions(GLFW_INCLUDE_NONE)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Platform_OpenGL
    SHARED
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Platform_OpenGL
    Alice_Log
    glad
    glfw
    glm
    stb
)

set_target_properties(Alice_Platform_OpenGL PROPERTIES OUTPUT_NAME "alice-platform-opengl")
