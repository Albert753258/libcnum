cmake_minimum_required(VERSION 3.0)
project(libcnum)

set(CMAKE_CXX_STANDARD 17)

add_executable(libcnum main.cpp
        include/libcomplexnumber.h
        src/fractionnum.cpp
        src/complexnumber.cpp
        src/expressionparser.cpp
)