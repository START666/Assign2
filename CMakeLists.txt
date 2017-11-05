cmake_minimum_required(VERSION 2.8.12.2)
project(Assign2)

set(CMAKE_CXX_CFLAGS -std=c++0x -pthread)

set(SOURCE_FILES main.cpp GameBoard.cpp GameBoard.h Minimax.cpp Minimax.h)
add_executable(Assign2 ${SOURCE_FILES})