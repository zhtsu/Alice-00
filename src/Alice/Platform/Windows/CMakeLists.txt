cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_compile_definitions(GLFW_INCLUDE_NONE)

add_library(Alice_Platform_Windows SHARED ${SRC_LIST})

target_link_libraries(Alice_Platform_Windows glfw Alice_Log glad)

set_target_properties(Alice_Platform_Windows PROPERTIES OUTPUT_NAME "Alice_Platform_Windows")
