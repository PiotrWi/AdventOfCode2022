#pragma once

#include <vector>

namespace year_2023::day11
{

using InputType = std::vector<std::vector<char> >;

InputType parse();

class Solution
{
public:
    long long solve(const InputType& input) const;
    long long solve_part2(const InputType& input) const;
};


}  // namespace year_2023::day11
