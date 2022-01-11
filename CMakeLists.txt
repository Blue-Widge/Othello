cmake_minimum_required(VERSION 3.17)
project(Othello C)

set(CMAKE_C_STANDARD 99)

add_executable(Othello main.c Board.c Board.h AI.c AI.h)