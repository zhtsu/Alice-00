cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Sandbox
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Sandbox
    Alice_Log
    Alice_Renderer
    Alice_Utils
    entt
)

set_target_properties(Sandbox PROPERTIES OUTPUT_NAME "sandbox")
