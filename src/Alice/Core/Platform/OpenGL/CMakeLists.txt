cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Platform_OpenGL
    SHARED
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Platform_OpenGL
    imgui
    glad
)

set_target_properties(Alice_Platform_OpenGL PROPERTIES OUTPUT_NAME "alice-platform-opengl")
