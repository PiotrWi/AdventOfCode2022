#pragma once

#include <vector>

namespace year_2023::day9
{

std::vector<std::vector<long long>> parse();

class Solution
{
public:
    long long solve(std::vector<std::vector<long long>>& input) const;
    long long solve_part2(std::vector<std::vector<long long>>& input) const;
};

}  // namespace year_2023::day9
