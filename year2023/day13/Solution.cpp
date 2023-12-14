#include "Solution.hpp"

#include <functional>
#include <ranges>
#include <numeric>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day13
{

InputType parse()
{
	InputType input;

	std::vector <std::string> in;
	for (auto&& line : parsers::getFile(2023, 13))
	{
		if (line.empty())
		{
			input.push_back(in);
			in = {};
			continue;
		}
		in.push_back(line);
	}
	if (!in.empty())
	{
		input.push_back(in);
	}
	return input;
}

namespace
{

int getNumOfDifferentCharsUpTo(const std::string& lhs, const std::string& rhs, int limit)
{
	int differentChars = 0;
	int i = 0;
	while (differentChars <= limit && i < lhs.size())
	{
		differentChars += (lhs[i] != rhs[i]);
		++i;
	}
	return differentChars;
}

bool check(int pos, const std::vector <std::string>& input, int limit)
{
	auto sum = 0;
	int j = 0;
	while (sum <= limit && pos - j >= 0 && pos + j + 1 < input.size())
	{
		sum += getNumOfDifferentCharsUpTo(input[pos - j], input[pos + j + 1], limit);
		++j;
	}
	return sum == limit;
}

int checkRowReflections(const std::vector <std::string>& input, int limit)
{
	for (int i = 0; i < input.size() - 1; ++i)
	{
		if (check(i, input, limit))
		{
			return i + 1;
		}
	}
	return 0;
}

long long solveSingle(const std::vector <std::string>& matrix, int limit)
{
	auto transposed = transpose(matrix);
	return checkRowReflections(matrix, limit) * 100 + checkRowReflections(transposed, limit);
}

long long solveForGivenReflectionCount(const InputType& input, int reflectionCount)
{
	auto range = input | std::views::transform(std::bind(solveSingle, std::placeholders::_1, reflectionCount));
	return std::accumulate(range.begin(), range.end(), 0ll);
}

}  // namespace

long long Solution::solve(const InputType& input) const
{
	return solveForGivenReflectionCount(input, 0);
}
long long Solution::solve_part2(const InputType& input) const
{
	return solveForGivenReflectionCount(input, 1);
}

}  // namespace year_2023::day13
