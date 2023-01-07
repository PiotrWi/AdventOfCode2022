#pragma once

#include <vector>

namespace day1
{

std::vector<std::vector<int>> parse();

class Solution
{
public:
    int solve(const std::vector<std::vector<int>>& input) const;
    int solve_part2(const std::vector<std::vector<int>>& input) const;
};

}  // namespace day1
