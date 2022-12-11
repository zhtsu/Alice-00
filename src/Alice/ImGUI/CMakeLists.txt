cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_ImGUI
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_ImGUI
    Alice_Layer
    imgui
    OpenGL::GL
    Alice_Application
    ImGuizmo
)

set_target_properties(Alice_ImGUI PROPERTIES OUTPUT_NAME "alice-imgui")
