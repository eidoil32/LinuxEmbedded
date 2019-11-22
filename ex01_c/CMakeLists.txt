cmake_minimum_required(VERSION 3.15)
project(ex01_c C)

set(CMAKE_C_STANDARD 11)

add_executable(ex01_c main.c MainChain.c MainChain.h Block.h Miscellaneous.h Miscellaneous.c Block.c)