cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Math
    SHARED
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Math
    glm
)

set_target_properties(Alice_Math PROPERTIES OUTPUT_NAME "alice-math")
