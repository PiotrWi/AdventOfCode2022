#pragma once

#include <vector>
#include <utility>

namespace day2
{

std::vector<std::pair<char, char>> parse();

class Solution
{
public:
    int solve(const std::vector<std::pair<char, char>>& input) const;
    int solve_part2(const std::vector<std::pair<char, char>>& input) const;
};

}
