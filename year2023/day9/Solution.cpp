#include "Solution.hpp"

#include <algorithm>
#include <string>
#include <ranges>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day9
{

InputType parse()
{
	InputType out;
	auto file = parsers::getFile(2023, 9);
	for (auto&& line : file)
	{
        auto elements = splitAndTrim(line, ' ');
		out.push_back(elements
			| std::views::transform([](const std::string& el) { return std::stoll(el); })
			| ToVector{}
		);
	}
	return out;
}

namespace
{

long long predictForward(const InputType::row_view& in)
{
	auto matrix = Matrix<long long>(in.size() + 1, in.size() + 1, 0ll);
	std::copy(in.begin(), in.end(), matrix.begin());

	for (auto row = 1u; row < matrix.rows_count(); ++row)
	{
		for (auto col = 0u; col < in.size() - row; ++col)
		{
			matrix[row][col] = matrix[row - 1][col + 1] - matrix[row - 1][col];
		}
	}

	for (auto col = 1u; col < matrix.cols_count(); ++col)
	{
		auto row = in.size() - col;
		matrix[row][col] = matrix[row][col - 1] + matrix[row + 1][col - 1];
	}
	return matrix[0][in.size()];
}

long long predictBackward(const InputType::row_view& in)
{
	auto matrix = Matrix<long long>(in.size() + 1, in.size() + 2, 0ll);
	std::copy(in.begin(), in.end(), matrix.begin() + 1);
	for (auto row = 1u; row < matrix.rows_count(); ++row)
	{
		for (auto col = 1u; col <= in.size() - row; ++col)
		{
			matrix[row][col] = matrix[row - 1][col + 1] - matrix[row - 1][col];
		}
	}

	for (int row = matrix.rows_count() - 2; row >= 0; --row)
	{
		matrix[row][0] = matrix[row][1] - matrix[row + 1][0];
	}
	return matrix[0][0];
}

}  // namespace

// 3163359954 to hight
long long Solution::solve(InputType& input) const
{
	auto sum = 0ll;
	for (auto&& row : input.rows())
	{
		sum += predictForward(row);
	}
	return sum;
}

long long Solution::solve_part2(InputType& input) const
{
	auto sum = 0ll;
	for (auto&& row : input.rows())
	{
		sum += predictBackward(row);
	}
	return sum;
}

}
