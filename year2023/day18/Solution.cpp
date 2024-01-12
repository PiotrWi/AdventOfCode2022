#include "Solution.hpp"

#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <ranges>

#include <utility/Martix.hpp>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day18
{

std::istream& operator >> (std::istream& is, Dir& dir)
{
	char c;
	is >> c;
	if (c == 'L')
	{
		dir = Dir::left;
	}
	if (c == 'R')
	{
		dir = Dir::right;
	}
	if (c == 'D')
	{
		dir = Dir::down;
	}
	if (c == 'U')
	{
		dir = Dir::up;
	}
	return is;
}

InputType parse()
{
	InputType in;
	for (auto&& line : parsers::getFile(2023, 18))
	{
		std::stringstream ss(line);
		MovementType mt;
		ss >> mt.dirrection;
		ss >> mt.lenght;
		ss >> mt.color;
		in.push_back(mt);
	}
	return in;
}

namespace
{

auto getListOfPixels(const InputType& in)
{
	std::map<int, std::set<int>> rowToCol;

	PointRowColOrientation cursor = { Dir::down, 1, 1 };

	for (auto&& elem : in)
	{
		cursor.orientation_ = elem.dirrection;
		for (int i = 0; i < elem.lenght; ++i)
		{
			cursor.position_ += getDirrectionDif(cursor.orientation_);
			rowToCol[cursor.position_.row].insert(cursor.position_.col);
		}
	}
	return rowToCol;
}

auto getPixelRanges(std::map<int, std::set<int>> rowToCol)
{
	auto minRow = rowToCol.begin()->first;
	auto maxRow = rowToCol.rbegin()->first;

	int maxCol = -1000;
	int minCol = 1000;

	for (auto elem : rowToCol)
	{
		auto& cols = elem.second;
		maxCol = std::max(maxCol, *cols.rbegin());
		minCol = std::min(minCol, *cols.begin());
	}
	return std::make_tuple(minRow, maxRow, minCol, maxCol);
}

auto createPixelMap(std::map<int, std::set<int>>& in)
{
	auto [minRow, maxRow, minCol, maxCol] = getPixelRanges(in);
	Matrix<char> pixMap(maxRow - minRow + 3, maxCol - minCol + 3, '.');
	for (auto&& elem : in)
	{
		auto& row = elem.first;
		for (auto&& col : elem.second)
		{
			pixMap[row - minRow + 1][col - minCol + 1] = '#';
		}
	}
	return pixMap;
}

void floodPixels(Matrix<char>& pixMap)
{
	auto queue = std::queue<PointRowCol>();
	queue.push(PointRowCol{ 0, 0 });

	while (not queue.empty())
	{
		auto pos = queue.front();
		queue.pop();
		auto& field = pixMap[pos];
		if (field == '.')
		{
			field = 'O';
			for (auto&& nextPos : getInBoundsNeighbours(pixMap, pos))
			{
				queue.push(nextPos);
			}
		}
	}
}

}  // namespace

long long Solution::solve(const InputType& in) const
{
	auto listOfPixels = getListOfPixels(in);
	auto pixMap = createPixelMap(listOfPixels);

	floodPixels(pixMap);

	return std::count_if(pixMap.begin(), pixMap.end(), [](char c) {return c == '#' || c == '.'; });
}

namespace
{

auto charToColor(char c)
{
	if (c == '0')
	{
		return Dir::right;
	}
	if (c == '1')
	{
		return Dir::down;
	}
	if (c == '2')
	{
		return Dir::left;
	}
	return Dir::up;
}

auto colorToInstruction(MovementType in)
{
	MovementType mt;
	auto lenStr = in.color.substr(2, 5);
	mt.lenght = std::stoi(lenStr, 0, 16);
	mt.dirrection = charToColor(in.color[7]);
	return mt;
}

auto getListOfIntermediatePixels(const InputType& in)
{
	std::vector<PointRowCol> out;

	PointRowColOrientation cursor = { Dir::down, 1, 1 };
	out.push_back(cursor.position_);
	for (auto&& line : in | std::views::transform(colorToInstruction))
	{
		cursor.position_ = cursor.position_ + line.lenght * getDirrectionDif(line.dirrection);
		out.push_back(cursor.position_);
	}
	return out;
}

struct Range
{
	int begin;
	int end;
};

auto createLegend(const std::set<int>& in)
{
	std::vector<Range> ranges;

	auto prev = in.begin();
	auto next = ++in.begin();
	ranges.emplace_back(-1000'000'000, *in.begin());
	for (; next != in.end(); ++prev, ++next)
	{
		ranges.emplace_back(*prev, *prev + 1);
		if (*next - *prev > 1)
		{
			ranges.emplace_back(*prev + 1, *next);
		}
	}
	ranges.emplace_back(*in.rbegin(), *in.rbegin()+1);
	ranges.emplace_back(*in.rbegin() + 1, 1000'000'000);

	return ranges;
}

auto getLegends(std::vector<PointRowCol>& in)
{
	std::set<int> rows;
	std::set<int> cols;
	for (auto& [row, col] : in)
	{
		rows.insert(row);
		cols.insert(col);
	}

	return std::make_tuple(createLegend(rows), createLegend(cols));
}

}  // namespace

long long Solution::solve_part2(const InputType& in) const
{
	auto listOfPixels = getListOfIntermediatePixels(in);
	auto [rowsLegend, colsLegend] = getLegends(listOfPixels);

	Matrix<char> pixMap(rowsLegend.size(), colsLegend.size(), '.');

	PointRowColOrientation cursor = { Dir::down, 1, 1 };
	PointRowColOrientation indexes;
	indexes.position_.row = std::ranges::find_if(rowsLegend, [](auto&& el) { return el.begin == 1; }) - rowsLegend.begin();
	indexes.position_.col = std::ranges::find_if(colsLegend, [](auto&& el) { return el.begin == 1; }) - colsLegend.begin();

	pixMap[indexes.position_.row][indexes.position_.col] = '#';

	for (auto&& entity : in | std::views::transform(colorToInstruction))
	{
		indexes.orientation_ = entity.dirrection;
		cursor.orientation_ = entity.dirrection;

		cursor.position_ += entity.lenght * getDirrectionDif(cursor.orientation_);

		while (rowsLegend[indexes.position_.row].begin != cursor.position_.row || colsLegend[indexes.position_.col].begin != cursor.position_.col)
		{
			indexes.position_ += getDirrectionDif(cursor.orientation_);
			pixMap[indexes.position_.row][indexes.position_.col] = '#';
		}
	}

	floodPixels(pixMap);

	auto sum = 0ull;
	for (auto it = pixMap.begin(); it != pixMap.end(); ++it)
	{
		sum += (*it == '#' || *it == '.')
			* (long long)(rowsLegend[it.getRow()].end - rowsLegend[it.getRow()].begin)
			* (long long)(colsLegend[it.getCol()].end - colsLegend[it.getCol()].begin);
	}

	return sum;
}

}  // namespace year_2023::day18

