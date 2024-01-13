#pragma once

#include <utility/Martix.hpp>

namespace day24
{

using Wall = Matrix<char>;

Wall parse();

struct Solution
{
    int solve(Wall);
    int solve_part2(Wall wall);
};

}

