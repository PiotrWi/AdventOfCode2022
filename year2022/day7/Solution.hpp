#pragma once

#include <string>
#include <vector>

namespace day7
{

struct Command
{
    std::string command_;
    std::vector<std::string> output;
};

std::vector<Command> parse();

class Solution
{
public:
    int solve(const std::vector<Command>&) const;
    int solve_part2(const std::vector<Command>&) const;
};

}  // namespace day7
