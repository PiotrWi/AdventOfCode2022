#pragma once

#include <string>
#include <vector>
#include <utility/PointRowColOrientation.hpp>

namespace year_2023::day18
{

struct MovementType
{
	Dir dirrection;
	int lenght;
	std::string color;
};

using InputType = std::vector<MovementType >;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day18
