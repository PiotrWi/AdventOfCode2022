cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

project(aoc LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -mbmi2 -mavx -march=haswell -fuse-ld=gold -lrt")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(COPY day1/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day1)
file(COPY day2/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day2)

add_executable(aoc
    main.cpp
    day1/sollution.cpp
    day2/sollution.cpp)

