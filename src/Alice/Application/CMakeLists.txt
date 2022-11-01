cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(
    Alice_Application
    STATIC
    ${SRC_LIST}
)

target_link_libraries(
    Alice_Application
    Alice_Log
    glad
    Alice_Layer
    Alice_Platform_Windows
)

set_target_properties(Alice_Application PROPERTIES OUTPUT_NAME "alice-application")
