#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <iterator>
#include <tuple>

#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day11
{

InputType parse()
{
	return parsers::getFile(2023, 11) | To<InputType>();
}

namespace
{

auto expandEmptyLines(const InputType& in)
{
	std::vector<int> emptyRows{0};
	std::vector<int> emptyCols{0};

	for (auto row = 1u; row < in.rows_count(); ++row)
	{
		if (std::ranges::none_of(in[row], [](auto c) { return c == '#'; }))
		{
			emptyRows.push_back(emptyRows[row - 1] + 1);
		}
		else
		{
			emptyRows.push_back(emptyRows[row - 1]);
		}
	}

	for (auto col = 1u; col < in.cols_count(); ++col)
	{
		bool hasHash = false;
		for (auto row = 0u; row < in.cols_count(); ++row)
		{
			hasHash |= in[row][col] == '#';
		}
		if (not hasHash)
		{
			emptyCols.push_back(emptyCols[col - 1] + 1);
		}
		else
		{
			emptyCols.push_back(emptyCols[col - 1]);
		}

	}
	return std::make_tuple(emptyRows, emptyCols);
}

auto getAllHashes(const InputType& in)
{
	std::vector<PointRowCol> hashes;
	for (auto it = in.begin(); it != in.end(); ++it)
	{
		if (*it == '#')
		{
			hashes.push_back(it.getPoint());
		}
	}
	return hashes;
}

auto calculate(const InputType& in, long long expansionSpeed)
{
	auto [emptyRows, emptyCols] = expandEmptyLines(in);
	auto allHashesVec = getAllHashes(in);

	auto sum = 0ll;
	for (auto i = 0u; i < allHashesVec.size(); i++)
	{
		for (auto j = i + 1; j < allHashesVec.size(); ++j)
		{
			auto minRow = std::min(allHashesVec[i].row, allHashesVec[j].row);
			auto maxRow = std::max(allHashesVec[i].row, allHashesVec[j].row);
			sum += maxRow - minRow + (emptyRows[maxRow] - emptyRows[minRow]) * expansionSpeed;

			auto minCol = std::min(allHashesVec[i].col, allHashesVec[j].col);
			auto maxCol = std::max(allHashesVec[i].col, allHashesVec[j].col);
			sum += maxCol - minCol + (emptyCols[maxCol] - emptyCols[minCol]) * expansionSpeed;
		}
	}
	return sum;
}

}  // namespace

long long Solution::solve(const InputType& in) const
{
	return calculate(in , 1);
}

long long Solution::solve_part2(const InputType& in) const
{
	return calculate(in, 999999ll);
}

}  // namespace year_2023::day11
