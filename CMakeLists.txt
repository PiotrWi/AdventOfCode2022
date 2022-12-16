cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

project(aoc LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -fuse-ld=gold")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(COPY day1/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day1)
file(COPY day2/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day2)
file(COPY day3/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day3)
file(COPY day4/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day4)
file(COPY day5/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day5)
file(COPY day6/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day6)
file(COPY day7/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day7)

add_executable(aoc
    main.cpp
    day1/Solution.cpp day1/Solution.hpp
    day2/Solution.cpp day2/Solution.hpp
    day3/Solution.cpp day3/Solution.hpp
    day4/Solution.cpp day4/Solution.hpp
    day5/Solution.cpp day5/Solution.hpp
    day6/Solution.cpp day6/Solution.hpp
    day7/Solution.cpp
    parsers/parsers.hpp)

