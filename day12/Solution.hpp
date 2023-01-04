#pragma once
#include <vector>

namespace day12
{

std::vector<std::vector<char>> parse();

struct Solution
{
public:
    int solve(std::vector<std::vector<char>>);
    int solve_part2(std::vector<std::vector<char>>);
};

}
