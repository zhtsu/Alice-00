cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Utils
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Utils
    glm
    glad
    Alice_Renderer
)

set_target_properties(Alice_Utils PROPERTIES OUTPUT_NAME "alice-utils")
