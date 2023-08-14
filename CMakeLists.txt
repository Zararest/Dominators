cmake_minimum_required(VERSION 3.0.0)
project(Dominators VERSION 0.1.0)

include_directories(include)

add_executable(Proto main.cpp)
target_link_libraries(Proto PRIVATE
                      NamedNode) 
target_link_libraries(Proto PRIVATE
                      Utils)                                                         

set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
add_definitions(-DWORKING_DIR="${BIN_DIR}")
                      
add_subdirectory(src)