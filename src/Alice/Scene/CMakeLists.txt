cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Scene
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Scene
    Alice_Log
    Alice_Renderer
    Alice_Core
    entt
    glm
    box2d
    yaml-cpp
)

set_target_properties(Alice_Scene PROPERTIES OUTPUT_NAME "alice-scene")
