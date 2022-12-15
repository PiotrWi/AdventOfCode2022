#include <iostream>

#include "day1/Solution.hpp"
#include "day2/Solution.hpp"
#include "day3/Solution.hpp"
#include "day4/Solution.hpp"
#include "day5/Solution.hpp"
#include "day6/Solution.hpp"

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
    return 0;
}
