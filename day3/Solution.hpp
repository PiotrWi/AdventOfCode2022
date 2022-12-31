#pragma once

#include <string>
#include <vector>

namespace day3
{

std::vector<std::string> parse();

class Solution
{
public:
    int solve(std::vector<std::string> input);
    int solve_part2(std::vector<std::string> in);
};

}  // namespace day3
