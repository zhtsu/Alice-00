cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_Events SHARED ${SRC_LIST})

set_target_properties(Alice_Events PROPERTIES OUTPUT_NAME "Alice_Events")
