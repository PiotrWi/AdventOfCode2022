#pragma once

#include <vector>
#include <string>

namespace year_2023::day1
{

std::vector<std::string> parse();

class Solution
{
public:
    int solve(const std::vector<std::string>& input) const;
    int solve_part2(const std::vector<std::string>& input) const;
};

}  // namespace year_2022::day1
