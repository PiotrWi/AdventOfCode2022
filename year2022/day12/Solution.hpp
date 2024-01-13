#pragma once

#include <utility/Martix.hpp>

namespace day12
{

using InputType = Matrix<char>;
InputType parse();

struct Solution
{
public:
    int solve(InputType);
    int solve_part2(InputType);
};

}
