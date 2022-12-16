#pragma once

#include <vector>

namespace day9
{
struct Command
{
    char dir;
    int n;
};

std::vector<Command> parse();

class Solution
{
public:
    int solve(const std::vector<Command>& in);
    int solve_part2(const std::vector<Command>& in);
};

}  // namespace day9
