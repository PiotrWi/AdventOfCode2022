#pragma once

#include <utility/Martix.hpp>

namespace day8
{

using InputType = Matrix<int>;
InputType parse();

class Solution
{
public:
    int solve(const InputType&);
    int solve_part2(const InputType&);
};

}  // namespace
