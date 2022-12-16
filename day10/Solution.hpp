#pragma once

#include <string>
#include <vector>

namespace day10
{

struct Command
{
    std::string command;
    int arg;
};

std::vector<Command> parse();

class Solution
{
public:
    int solve(const std::vector<Command>& in);
    int solve_part2(const std::vector<Command>& in);
};

}  // namespace day10
