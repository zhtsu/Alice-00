cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(Alice_Log SHARED ${SRC_LIST})

target_link_libraries(Alice_Log spdlog)

set_target_properties(Alice_Log PROPERTIES OUTPUT_NAME "Alice_Log")
