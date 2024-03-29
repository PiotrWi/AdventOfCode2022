#pragma once

#include <array>
#include <vector>

#include <utility/PointXYZ.hpp>

namespace year_2023::day22
{

struct Brick
{
	static Brick construct(PointXYZ first, PointXYZ second);

	void addLowerPoints(std::vector<int> lowers);
	void addUpperIndex(int upper);

	PointXYZ lowerPoint_;
	PointXYZ upperPoint_;

	std::vector<int> blocksUpper_;
	std::vector<int> blocksLower_;
};

using InputType = std::vector<Brick>;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day22
