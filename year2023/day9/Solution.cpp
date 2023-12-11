#include "Solution.hpp"

#include <algorithm>
#include <string>
#include <ranges>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day9
{

std::vector<std::vector <long long >> parse()
{
	std::vector<std::vector <long long >> out;
	auto file = parsers::getFile(2023, 9);
	for (auto&& line : file)
	{
		out.emplace_back(
			splitAndTrim(line, ' ')
			| std::views::transform([](auto&& el) { return std::stoll(el); })
			| ToVector{}
		);
	}
	return out;
}

namespace
{

long long predictForward(const std::vector<long long>& in)
{
	auto matrix = std::vector <std::vector<long long>>(in.size() + 1, std::vector<long long>(in.size() + 1, 0ll));
	std::copy(in.begin(), in.end(), matrix[0].begin());

	for (auto row = 1; row < matrix.size(); ++row)
	{
		for (auto col = 0; col < in.size() - row; ++ col)
		{
			matrix[row][col] = matrix[row - 1][col + 1] - matrix[row - 1][col];
		}
	}

	for (int col = 1; col < matrix[0].size(); ++col)
	{
		auto row = in.size() - col;
		matrix[row][col] = matrix[row][col - 1] + matrix[row + 1][col - 1];
	}
	return matrix[0][in.size()];
}

long long predictBackward(const std::vector<long long>& in)
{
	auto matrix = std::vector <std::vector<long long>>(in.size() + 1, std::vector<long long>(in.size() + 2, 0ll));
	std::copy(in.begin(), in.end(), matrix[0].begin() + 1);
	for (auto row = 1; row < matrix.size(); ++row)
	{
		for (auto col = 1; col <= in.size() - row; ++col)
		{
			matrix[row][col] = matrix[row - 1][col + 1] - matrix[row - 1][col];
		}
	}

	for (int row = matrix.size() - 2; row >= 0; --row)
	{
		matrix[row][0] = matrix[row][1] - matrix[row + 1][0];
	}
	return matrix[0][0];
}

}  // namespace

// 3163359954 to hight
long long Solution::solve(std::vector<std::vector<long long>>& input) const
{
	auto sum = 0ll;
	for (auto&& row : input)
	{
		sum += predictForward(row);
	}
	return sum;
}

long long Solution::solve_part2(std::vector<std::vector<long long>>& input) const
{
	auto sum = 0ll;
	for (auto&& row : input)
	{
		sum += predictBackward(row);
	}
	return sum;
}

}
