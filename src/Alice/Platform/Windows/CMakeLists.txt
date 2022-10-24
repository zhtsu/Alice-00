cmake_minimum_required(VERSION 3.23.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_Platform_Windows SHARED ${SRC_LIST})

target_link_libraries(Alice_Platform_Windows glfw Alice_Log)

set_target_properties(Alice_Platform_Windows PROPERTIES OUTPUT_NAME "Alice_Platform_Windows")
