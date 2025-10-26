cmake_minimum_required(VERSION 3.10)
project(hash_table)

add_library(hash_library STATIC src/hash_table.c)

target_include_directories(hash_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(hash_binary src/main.c)

target_link_libraries(hash_binary PRIVATE hash_library)
