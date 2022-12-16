#pragma once

#include <vector>

namespace day8
{

std::vector<std::vector<int>> parse();

class Solution
{
public:
    int solve(const std::vector<std::vector<int>>&);
    int solve_part2(const std::vector<std::vector<int>>&);
};

}  // namespace
