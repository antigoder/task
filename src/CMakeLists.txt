cmake_minimum_required(VERSION 2.8)

project(TaskPool)

file(GLOB_RECURSE source_list FOLLOW_SYMLINKS core/*.cpp util/*.cpp)

add_definitions("-std=c++0x -O -g") 

include_directories(${PROJECT_SOURCE_DIR}/deps)
include_directories(${PROJECT_SOURCE_DIR}/core)
include_directories(${PROJECT_SOURCE_DIR}/util)
add_executable(main core/main.cpp ${source_list})
#set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(main pthread)
