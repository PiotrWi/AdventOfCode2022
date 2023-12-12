#pragma once

#include <string>
#include <vector>

namespace year_2023::day12
{

struct Line
{
	std::string line;
	std::vector<int> groups;
};

using InputType = std::vector<Line>;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};


}  // namespace year_2023::day12s
