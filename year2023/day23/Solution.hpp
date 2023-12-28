#pragma once

#include <vector>
#include <string>

namespace year_2023::day23
{

using InputType = std::vector<std::string>;

InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day23
