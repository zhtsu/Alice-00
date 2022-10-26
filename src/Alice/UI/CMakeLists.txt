cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_UI SHARED ${SRC_LIST})

target_link_libraries(
    Alice_UI
    imgui
    Alice_Layer
    glfw
    Alice_Platform_OpenGL
    Alice_Application
)

set_target_properties(Alice_UI PROPERTIES OUTPUT_NAME "Alice_UI")
