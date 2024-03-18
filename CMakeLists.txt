cmake_minimum_required(VERSION 3.19)
project(CodeStat C)

set(CMAKE_C_STANDARD 11)

add_executable(CodeStat main.c file.c class.c str.c)