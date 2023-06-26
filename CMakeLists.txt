cmake_minimum_required(VERSION 3.22)
project(sudoku)
# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "-static")
include_directories(include)
add_executable(sudoku
        include/Board.h
        include/Generator.h
        include/Solver.h
        src/Board.cpp
        src/Generator.cpp
        src/Solver.cpp
        main.cpp)
