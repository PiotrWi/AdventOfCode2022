#pragma once

#include <vector>

namespace day24
{

using Wall = std::vector<std::vector<char>>;

Wall parse();

struct Solution
{
    int solve(Wall);
    int solve_part2(Wall wall);
};

}

