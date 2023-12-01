#pragma once

#include <vector>

namespace day4
{

using TEntityType = std::pair<std::pair<int, int>, std::pair<int, int>>;

std::vector<TEntityType> parse();

class Solution
{
public:
    int solve(const std::vector<TEntityType>& input) const;
    int solve_part2(const std::vector<TEntityType>& input) const;
};

}  // namespace day4
