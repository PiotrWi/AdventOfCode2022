#pragma once

#include <vector>

namespace day17
{

enum class WindDirrection
{
    left,
    right
};

std::vector<WindDirrection> parse();

struct Solution
{
    int solve(std::vector<WindDirrection>);
    long long solve_part2(std::vector<WindDirrection> in);
};

}
