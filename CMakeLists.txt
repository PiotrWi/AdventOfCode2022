cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

project(aoc LANGUAGES CXX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -fuse-ld=gold")
else()
    SET(CMAKE_CXX_FLAGS  "-Ofast -Wall -Wextra -Werror -std=c++20 -mbmi2 -mavx -march=haswell -msse4.1 -flto -fuse-ld=gold -m64 -lrt")
endif()

SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -fuse-ld=gold")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(COPY day1/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day1)
file(COPY day2/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day2)
file(COPY day3/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day3)
file(COPY day4/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day4)
file(COPY day5/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day5)
file(COPY day6/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day6)
file(COPY day7/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day7)
file(COPY day8/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day8)
file(COPY day9/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day9)
file(COPY day10/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day10)
file(COPY day11/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day11)
file(COPY day12/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day12)
file(COPY day13/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day13)
file(COPY day14/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day14)
file(COPY day15/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day15)
file(COPY day16/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day16)
file(COPY day17/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day17)
file(COPY day18/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day18)
file(COPY day19/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day19)
file(COPY day20/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day20)
file(COPY day21/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day21)
file(COPY day22/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day22)
file(COPY day23/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day23)
file(COPY day24/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day24)
file(COPY day25/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/day25)

add_executable(aoc
        main.cpp
        parsers/parsers.hpp
        StringAlgorithms/StringAlgorithms.hpp
        utility/PointXY.hpp utility/PointXY.cpp
        day1/Solution.cpp day1/Solution.hpp
        day2/Solution.cpp day2/Solution.hpp
        day3/Solution.cpp day3/Solution.hpp
        day4/Solution.cpp day4/Solution.hpp
        day5/Solution.cpp day5/Solution.hpp
        day6/Solution.cpp day6/Solution.hpp
        day7/Solution.cpp day7/Solution.hpp
        day8/Solution.hpp day8/Solution.cpp
        day9/Solution.hpp day9/Solution.cpp
        day10/Solution.hpp day10/Solution.cpp
        day11/Solution.cpp day11/Solution.hpp
        day12/Solution.cpp day12/Solution.hpp
        day13/Solution.hpp day13/Solution.cpp
        day14/Solution.hpp day14/Solution.cpp
        day14/Solution.hpp day14/Solution.cpp
        day15/Solution.hpp day15/Solution.cpp
        day16/Solution.hpp day16/Solution.cpp
        day17/Solution.hpp day17/Solution.cpp
        day18/Solution.hpp day18/Solution.cpp
        day19/Solution.hpp day19/Solution.cpp
        day20/Solution.hpp day20/Solution.cpp
        day21/Solution.hpp day21/Solution.cpp day22/Solution.hpp day22/Solution.cpp parsers/parsers.cpp StringAlgorithms/StringAlgorithms.cpp day23/Solution.hpp day23/Solution.cpp utility/PointRowCol.hpp utility/PointRowCol.cpp day24/Solution.hpp day24/Solution.cpp utility/PointRowColOrientation.hpp utility/PointRowColOrientation.cpp day25/Solution.hpp day25/Solution.cpp)

