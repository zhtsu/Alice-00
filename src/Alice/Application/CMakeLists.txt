cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_Application SHARED ${SRC_LIST})

target_link_libraries(
    Alice_Application
    Alice_Log
    Alice_Platform_Windows
    OpenGL::GL
    Alice_Layer
)

set_target_properties(Alice_Application PROPERTIES OUTPUT_NAME "Alice_Application")
