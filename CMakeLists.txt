cmake_minimum_required(VERSION 3.17)
project(minkova)

set(CMAKE_CXX_STANDARD 17)

add_executable(splay_tree 2_module/splay_tree.cpp)
add_executable(heap 2_module/min_heap.cpp)

add_executable(knapsack 3_module/knapsack.cpp)
add_executable(crazy_man 3_module/crazy_man.cpp)
add_executable(bloom_filter 3_module/bloom_filter.cpp)
