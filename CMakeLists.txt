cmake_minimum_required(VERSION 3.0.0)
project(Dominators VERSION 0.1.0)

include_directories(include)

add_executable(Proto main.cpp)                       

add_subdirectory(src)