cmake_minimum_required(VERSION 2.8.12.2)
project(Assign2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(SOURCE_FILES main.cpp GameBoard.cpp GameBoard.h Minimax.cpp Minimax.h)
add_executable(Assign2 ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Assign2 pthread)