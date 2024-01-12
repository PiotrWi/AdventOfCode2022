#pragma once

#include <utility/Martix.hpp>

namespace year_2023::day17
{

using InputType = Matrix<int>;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day17
