cmake_minimum_required(VERSION 2.8) 

project(TaskPool)

file(GLOB_RECURSE source_list FOLLOW_SYMLINKS main/*.cpp worker/*.cpp util/*.cpp db/*.cpp)

add_definitions("-std=c++11 -O -g") 

include_directories(${PROJECT_SOURCE_DIR}/worker
                    ${PROJECT_SOURCE_DIR}/util
                    ${PROJECT_SOURCE_DIR}/db
                    ${PROJECT_SOURCE_DIR}/deps/include
                    ${PROJECT_SOURCE_DIR}/deps/gcc/gcc-5.3/include)

link_directories(${PROJECT_SOURCE_DIR}/deps/gcc/gcc-5.3/lib64
                 ${PROJECT_SOURCE_DIR}/deps/lib/leveldb)

add_executable(main main/main.cpp ${source_list})
#set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(main pthread leveldb)

