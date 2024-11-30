cmake_minimum_required(VERSION 3.28.1)
project(CPlane)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPlane
        main.cpp
        tokenizer.cpp tokenizer.h
        tokens.h symbols.h
        errors.h errors.cpp
        exec_path.h exec_path.cpp
        symbol_table.h symbol_table.cpp
        ast.h ast.cpp
        recursive_descent_parser.h recursive_descent_parser.cpp
        interpreter.h interpreter.cpp
        eval.h eval.cpp
)
