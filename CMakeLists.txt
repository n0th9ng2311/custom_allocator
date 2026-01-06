cmake_minimum_required(VERSION 3.28)
project(working C)

set(CMAKE_C_STANDARD 11)

add_executable(working main.c
        allocator.c
        allocator.h)
