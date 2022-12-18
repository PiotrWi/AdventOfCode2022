#pragma once
#include <vector>

namespace day12
{

std::vector<std::vector<int>> parse();

struct Solution
{
public:
    int solve(std::vector<std::vector<int>>);
    int solve_part2(std::vector<std::vector<int>>);
};

}
