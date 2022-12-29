#include <iostream>

#include "day1/Solution.hpp"
#include "day2/Solution.hpp"
#include "day3/Solution.hpp"
#include "day4/Solution.hpp"
#include "day5/Solution.hpp"
#include "day6/Solution.hpp"
#include "day7/Solution.hpp"
#include "day8/Solution.hpp"
#include "day9/Solution.hpp"
#include "day10/Solution.hpp"
#include "day11/Solution.hpp"
#include "day12/Solution.hpp"
#include "day13/Solution.hpp"
#include "day14/Solution.hpp"
#include "day15/Solution.hpp"
#include "day16/Solution.hpp"
#include "day17/Solution.hpp"
#include "day18/Solution.hpp"
#include "day19/Solution.hpp"
#include "day20/Solution.hpp"
#include "day21/Solution.hpp"
#include "day22/Solution.hpp"

template <typename T>
T clone(const T& t)
{
    return t;
}

int main()
{
    {
        day1::Solution s;
        auto intput = day1::parse();
        std::cout << "day 1 part 1: " << s.solve(intput) << std::endl;
        std::cout << "day 1 part 2: " << s.solve_part2(intput) << std::endl;
    }
    {
        day2::Solution s;
        auto intput = day2::parse();
        std::cout << "day 2 test 1: " << s.solve({{'A', 'Y'},
                                                  {'B', 'X'},
                                                  {'C', 'Z'}}) << std::endl;
        std::cout << "day 2 part 1: " << s.solve(intput) << std::endl;
        std::cout << "day 2 part 2: " << s.solve_part2(intput) << std::endl;

    }
    {
        day3::Solution s;
        auto input = day3::parse();
        std::cout << "day 3 test 1: " << s.solve({"vJrwpWtwJgWrhcsFMMfFFhFp",
                                                         "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
                                                         "PmmdzqPrVvPwwTWBwg",
                                                         "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
                                                         "ttgJtRGJQctTZtZT",
                                                         "CrZsJsPPZsGzwwsLwLmpwMDw"}) << std::endl;
        std::cout << "day 3 part 1: " << s.solve(clone(input)) << std::endl;
        std::cout << "day 3 part 2: " << s.solve_part2(std::move(input)) << std::endl;
    }
    {
        day4::Solution s;
        auto input = day4::parse();
        std::cout << "day 4 part1: " << s.solve(input) << std::endl;
        std::cout << "day 4 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day5::Solution s;
        auto input = day5::parse();
        std::cout << "day 5 part1: " << s.solve(input) << std::endl;
        std::cout << "day 5 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day6::Solution s;
        auto input = day6::parse();
        std::cout << "day 6 part1: " << s.solve(input) << std::endl;
        std::cout << "day 6 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day7::Solution s;
        auto input = day7::parse();
        std::cout << "day 7 part1: " << s.solve(input) << std::endl;
        std::cout << "day 7 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day8::Solution s;
        auto input = day8::parse();
        std::cout << "day 8 part1: " << s.solve(input) << std::endl;
        std::cout << "day 8 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day9::Solution s;
        auto input = day9::parse();
        std::cout << "day 9 part1: " << s.solve(input) << std::endl;
        std::cout << "day 9 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day10::Solution s;
        auto input = day10::parse();
        std::cout << "day 10 part1: " << s.solve(input) << std::endl;
        s.solve_part2(input);
    }
    {
        day11::Solution s;
        auto input = day11::parse();
        std::cout << "day 11 part1: " << s.solve(input) << std::endl;
        std::cout << "day 11 part1: " << s.solve_part2(input) << std::endl;
    }
    {
        day12::Solution s;
        auto input = day12::parse();
        std::cout << "day 12 part1: " << s.solve(input) << std::endl;
        std::cout << "day 12 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day13::Solution s;
        auto input = day13::parse();
        std::cout << "day 13 part1: " << s.solve(input) << std::endl;
        std::cout << "day 13 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day14::Solution s;
        auto input = day14::parse();
        std::cout << "day 14 part1: " << s.solve(input) << std::endl;
        std::cout << "day 14 part1: " << s.solve_part2(input) << std::endl;

    }
    {
        day15::Solution s;
        auto input = day15::parse();
        std::cout << "day 15 part1: " << s.solve(input) << std::endl;
        // std::cout << "day 15 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day16::Solution s;
        auto input = day16::parse();
        std::cout << "day 16 part1: " << s.solve(input) << std::endl;
        std::cout << "day 16 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day17::Solution s;
        auto input = day17::parse();
        std::cout << "day 17 part1: " << s.solve(input) << std::endl;
        std::cout << "day 17 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day18::Solution s;
        auto input = day18::parse();
        std::cout << "day 18 part1: " << s.solve(input) << std::endl;
        std::cout << "day 18 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day19::Solution s;
        auto input = day19::parse();
        std::cout << "day 19 part1: " << s.solve(input) << std::endl;
        std::cout << "day 19 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day20::Solution s;
        auto input = day20::parse();
        std::cout << "day 20 part1: " << s.solve(input) << std::endl;
        std::cout << "day 20 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        day21::Solution s;
        auto input = day21::parse();
        std::cout << "day 21 part1: " << s.solve(input) << std::endl;
        std::cout << "day 21 part2: " << s.solve_part2(input) << std::endl;
    }
    {
        // day22::Solution s;
        auto input = day22::parse();
        //std::cout << "day 22 part1: " << s.solve(input) << std::endl;
        //std::cout << "day 22 part2: " << s.solve_part2(input) << std::endl;
    }
    return 0;
}
