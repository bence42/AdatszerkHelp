cmake_minimum_required (VERSION 3.4)

project(khf01)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

enable_testing()

add_executable(khf01
  main.cpp
        problem1.hpp
        problem2.hpp)

add_test(NAME khf01_basetest
  COMMAND khf01 -v)
