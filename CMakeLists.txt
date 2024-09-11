cmake_minimum_required(VERSION 3.28.1)
project(CPlane)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPlane
        main.cpp
        tokens.h tokens.cpp
        tokenizer.h tokenizer.cpp
        errors.h errors.cpp
        exec_path.h exec_path.cpp
)
