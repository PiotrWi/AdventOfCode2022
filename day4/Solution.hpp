#pragma once

#include <vector>

namespace day4
{

using TEntityType = std::pair<std::pair<int, int>, std::pair<int, int>>;

std::vector<TEntityType> parse();

class Solution
{
public:
    int solve(const std::vector<TEntityType>& input);
    int solve_part2(const std::vector<TEntityType>& input);
};

}  // namespace day4
