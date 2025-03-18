cmake_minimum_required(VERSION 3.30)
project(memory_bugs)

set(CMAKE_CXX_STANDARD 20)

add_executable(memory_bugs bug3.cpp
        bug4.cpp)
