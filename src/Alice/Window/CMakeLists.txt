cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_Window SHARED ${SRC_LIST})

set_target_properties(Alice_Window PROPERTIES OUTPUT_NAME "Alice_Window")
