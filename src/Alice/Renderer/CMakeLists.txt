cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Renderer
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Renderer
    glad
    Alice_Log
    Alice_Platform_OpenGL
    Alice_Platform_Windows
    Alice_Scene
)

set_target_properties(Alice_Renderer PROPERTIES OUTPUT_NAME "alice-renderer")
