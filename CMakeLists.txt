cmake_minimum_required(VERSION 3.5 FATAL_ERROR)


find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

project(aoc LANGUAGES CXX)

if (CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(CMAKE_CXX_FLAGS  "-Og -g -Wall -Wextra -Werror -std=c++20 -march=haswell -fuse-ld=gold")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        SET(CMAKE_CXX_FLAGS  "-Ofast -Wall -Wextra -Werror -std=c++20 -mbmi2 -mavx -march=haswell -msse4.1 -flto -fuse-ld=gold -m64 -lrt")
    endif()
endif()

if(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(COPY year2022/day1/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day1)
file(COPY year2022/day2/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day2)
file(COPY year2022/day3/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day3)
file(COPY year2022/day4/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day4)
file(COPY year2022/day5/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day5)
file(COPY year2022/day6/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day6)
file(COPY year2022/day7/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day7)
file(COPY year2022/day8/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day8)
file(COPY year2022/day9/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day9)
file(COPY year2022/day10/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day10)
file(COPY year2022/day11/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day11)
file(COPY year2022/day12/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day12)
file(COPY year2022/day13/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day13)
file(COPY year2022/day14/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day14)
file(COPY year2022/day15/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day15)
file(COPY year2022/day16/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day16)
file(COPY year2022/day17/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day17)
file(COPY year2022/day18/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day18)
file(COPY year2022/day19/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day19)
file(COPY year2022/day20/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day20)
file(COPY year2022/day21/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day21)
file(COPY year2022/day22/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day22)
file(COPY year2022/day23/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day23)
file(COPY year2022/day24/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day24)
file(COPY year2022/day25/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2022/day25)

file(COPY year2023/day1/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2023/day1)
file(COPY year2023/day2/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2023/day2)
file(COPY year2023/day3/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2023/day3)
file(COPY year2023/day4/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/year2023/day4)

add_executable(aoc
        main.cpp
        parsers/parsers.hpp parsers/parsers.cpp
        StringAlgorithms/StringAlgorithms.hpp StringAlgorithms/StringAlgorithms.cpp
        utility/PointXY.hpp utility/PointXY.cpp
        utility/PointRowCol.hpp utility/PointRowCol.cpp
        utility/PointRowColOrientation.hpp utility/PointRowColOrientation.cpp
        year2022/day1/Solution.cpp year2022/day1/Solution.hpp
        year2022/day2/Solution.cpp year2022/day2/Solution.hpp
        year2022/day3/Solution.cpp year2022/day3/Solution.hpp
        year2022/day4/Solution.cpp year2022/day4/Solution.hpp
        year2022/day5/Solution.cpp year2022/day5/Solution.hpp
        year2022/day6/Solution.cpp year2022/day6/Solution.hpp
        year2022/day7/Solution.cpp year2022/day7/Solution.hpp
        year2022/day8/Solution.hpp year2022/day8/Solution.cpp
        year2022/day9/Solution.hpp year2022/day9/Solution.cpp
        year2022/day10/Solution.hpp year2022/day10/Solution.cpp
        year2022/day11/Solution.cpp year2022/day11/Solution.hpp
        year2022/day12/Solution.cpp year2022/day12/Solution.hpp
        year2022/day13/Solution.hpp year2022/day13/Solution.cpp
        year2022/day14/Solution.hpp year2022/day14/Solution.cpp
        year2022/day14/Solution.hpp year2022/day14/Solution.cpp
        year2022/day15/Solution.hpp year2022/day15/Solution.cpp
        year2022/day16/Solution.hpp year2022/day16/Solution.cpp
        year2022/day17/Solution.hpp year2022/day17/Solution.cpp
        year2022/day18/Solution.hpp year2022/day18/Solution.cpp
        year2022/day19/Solution.hpp year2022/day19/Solution.cpp
        year2022/day20/Solution.hpp year2022/day20/Solution.cpp
        year2022/day21/Solution.hpp year2022/day21/Solution.cpp
        year2022/day22/Solution.hpp year2022/day22/Solution.cpp
        year2022/day23/Solution.hpp year2022/day23/Solution.cpp
        year2022/day24/Solution.hpp year2022/day24/Solution.cpp
        year2022/day25/Solution.hpp year2022/day25/Solution.cpp
        year2023/day1/Solution.hpp year2023/day1/Solution.cpp
        year2022/execute.cpp
        year2023/execute.cpp  "year2023/day2/Solution.cpp" "year2023/day3/Solution.cpp" "year2023/day4/Solution.cpp")

set_property(TARGET aoc PROPERTY CXX_STANDARD 20)