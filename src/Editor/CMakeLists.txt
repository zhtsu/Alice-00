cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)
aux_source_directory(Panel PANEL_SRC_LIST)

add_library(
    Editor
    STATIC
    ${SRC_LIST}
    ${PANEL_SRC_LIST}
)

target_link_libraries(
    Editor
    Alice_Log
    Alice_Renderer
    Alice_Utils
    Alice_Scene
    # Alice_Math
)

set_target_properties(Editor PROPERTIES OUTPUT_NAME "editor")
