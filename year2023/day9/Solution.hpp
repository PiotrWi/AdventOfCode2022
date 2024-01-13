#pragma once

#include <utility/Martix.hpp>

namespace year_2023::day9
{

using InputType = Matrix<long long>;
InputType parse();

class Solution
{
public:
    long long solve(InputType& input) const;
    long long solve_part2(InputType& input) const;
};

}  // namespace year_2023::day9
