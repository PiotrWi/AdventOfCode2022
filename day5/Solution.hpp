#pragma once

#pragma once
#include "Solution.hpp"
#include <vector>
#include <string>

namespace day5
{

struct Move
{
    int from;
    int to;
    int count;
};

struct Input
{
    std::vector< std::vector<char> > stacks {9};
    std::vector<Move> commands;
};

Input parse();

class Solution
{
public:
    std::string solve(Input input);
    std::string  solve_part2(Input input);
};

}  // namespace day5
