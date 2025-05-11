cmake_minimum_required(VERSION 3.0)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp
        libcomplexnumber/libcomplexnumber.cpp
        libcomplexnumber/libcomplexnumber.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)
target_link_libraries(untitled PkgConfig::GMPXX)