cmake_minimum_required(VERSION 3.16.0)

aux_source_directory(. SRC_LIST)

add_library(imgui SHARED ${SRC_LIST})

target_include_directories(imgui PUBLIC .)

set_target_properties(imgui PROPERTIES OUTPUT_NAME "imgui")
