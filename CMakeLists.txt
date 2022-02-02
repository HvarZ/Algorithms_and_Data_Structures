cmake_minimum_required(VERSION 3.17)
project(farid_labs)

set(CMAKE_CXX_STANDARD 17)

add_executable(1.A 1_module/A.cpp)
add_executable(1.B 1_module/B.cpp)
add_executable(1.C 1_module/C.cpp)
add_executable(1.D 1_module/D.cpp)

add_executable(2.B 2_module/B.cpp)
add_executable(2.C 2_module/C.cpp)
add_executable(2.D 2_module/D.cpp)

add_executable(3.A 3_module/A.cpp)
add_executable(3.B 3_module/B.cpp)
add_executable(3.C 3_module/C.cpp)
