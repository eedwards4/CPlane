cmake_minimum_required(VERSION 3.28.1)
project(CPlane)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPlane
        main.cpp
        tokens.h
        tokens.cpp
        tokenizer.h
        tokenizer.cpp)
