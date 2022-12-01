cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Scene
    SHARED
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Scene
    entt
)

set_target_properties(Alice_Scene PROPERTIES OUTPUT_NAME "alice-scene")
