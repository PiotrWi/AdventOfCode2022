#pragma once

#include <string>
#include <vector>

namespace day22
{
enum CommandType
{
    GoForward,
    RotateLeft,
    RotateRight,
};
struct Command
{
    CommandType type;
    int count;
};

struct Input
{
    std::vector<std::string> map_;
    std::vector<Command> command_;
};

Input parse();

struct Solution
{
    int solve(Input);
    int solve_part2(Input);
};

}
