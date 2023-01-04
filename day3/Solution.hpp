#pragma once

#include <string>
#include <vector>

namespace day3
{

std::vector<std::string> parse();

class Solution
{
public:
    int solve(std::vector<std::string> input) const;
    int solve_part2(std::vector<std::string> in) const;
};

}  // namespace day3
