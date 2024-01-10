#pragma once

#include <vector>
#include <utility/PointXYZ.hpp>

namespace year_2023::day24
{

struct Cube
{
	PointXYZ position_;
	PointXYZ velocity_;
};

using InputType = std::vector<Cube>;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day24
