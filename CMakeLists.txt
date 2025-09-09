cmake_minimum_required(VERSION 3.10)
project(CFormatter CXX)
set(CMAKE_CXX_STANDARD 11)

add_executable(CFormatter
    ./ast/run.cpp
    ./lexer-parser/Lexer-Paser.hpp
    ./lexer-parser/ASTNodes.hpp
)