#pragma once

#include <string>
#include <vector>

namespace year_2023::day16
{

using InputType = std::vector<std::string >;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day16