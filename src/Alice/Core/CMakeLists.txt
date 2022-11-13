cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Core
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Core
    Alice_Platform_Windows
    Alice_Renderer
    glm
)

set_target_properties(Alice_Core PROPERTIES OUTPUT_NAME "alice-core")
