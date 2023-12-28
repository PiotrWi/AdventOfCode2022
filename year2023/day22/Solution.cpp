#include "Solution.hpp"

#include <string>
#include <queue>
#include <set>
#include <ranges>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day22
{

PointXYZ::PointXYZ(int x, int y, int z)
	: x_(x)
	, y_(y)
	, z_(z)
{
}

bool operator< (const PointXYZ& lhs, const PointXYZ& rhs)
{
	return lhs.z_ < rhs.z_;
}

bool operator< (const Brick& lhs, const Brick& rhs)
{
	return lhs.lowerPoint_ < rhs.lowerPoint_;
}

PointXYZ::PointXYZ(std::vector<int> array)
	: PointXYZ(array[0], array[1], array[2])
{
}

Brick Brick::construct(PointXYZ first, PointXYZ second)
{
	if (first.z_ <= second.z_)
	{
		return Brick{ first, second };
	}
	return Brick{ second, first };
}

void Brick::addLowerPoints(std::vector<int> lowers)
{
	blocksLower_ = lowers;
}

void Brick::addUpperIndex(int upper)
{
	blocksUpper_.push_back(upper);
}

auto createPointFromStr(std::string s)
{
	return PointXYZ(splitAndTrim(s, ',')
		| std::views::transform([](auto&& el) {return std::stoi(el); })
		| To<std::vector<int>>());
}

InputType parse()
{
	InputType input;
	auto file = parsers::getFile(2023, 22);

	for (auto&& line : file)
	{
		auto pointStrs = splitAndTrim(line, '~');
		input.emplace_back(
			Brick::construct(
				createPointFromStr(pointStrs[0]),
				createPointFromStr(pointStrs[1])));
	}

	return input;
}

auto findValuesRange(const InputType& input)
{
	auto maxX = 0;
	auto maxY = 0;
	for (auto&& element : input)
	{
		maxX = std::max(maxX, element.lowerPoint_.x_);
		maxX = std::max(maxX, element.upperPoint_.x_);
		maxY = std::max(maxY, element.lowerPoint_.y_);
		maxY = std::max(maxY, element.upperPoint_.y_);
	}
	return std::make_tuple(maxX, maxY);
}

struct BricksReferences
{
	int high = 0; 
	int brickIndex = -1;
};

std::vector<Brick> prepare(const InputType& input)
{
	auto [maxX, maxY] = findValuesRange(input);

	std::vector<Brick> lowerBricks = input;
	std::sort(lowerBricks.begin(), lowerBricks.end());

	std::vector<std::vector<BricksReferences>> topsOfBricks_(maxX + 1, std::vector<BricksReferences>(maxY + 1));

	for (int i = 0; i < lowerBricks.size(); ++i)
	{
		int biggestTop = -1;
		std::set<int> brickIndexes;
		for (auto x = lowerBricks[i].lowerPoint_.x_; x <= lowerBricks[i].upperPoint_.x_; ++x)
		{
			for (auto y = lowerBricks[i].lowerPoint_.y_; y <= lowerBricks[i].upperPoint_.y_; ++y)
			{
				if (topsOfBricks_[x][y].high > biggestTop)
				{
					brickIndexes = {};
					biggestTop = topsOfBricks_[x][y].high;
					;
				}
				if (topsOfBricks_[x][y].high == biggestTop && topsOfBricks_[x][y].brickIndex != -1)
				{
					brickIndexes.insert(topsOfBricks_[x][y].brickIndex);
				}
			}
		}

		for (auto x = lowerBricks[i].lowerPoint_.x_; x <= lowerBricks[i].upperPoint_.x_; ++x)
		{
			for (auto y = lowerBricks[i].lowerPoint_.y_; y <= lowerBricks[i].upperPoint_.y_; ++y)
			{
				topsOfBricks_[x][y].high = biggestTop + 1 + (lowerBricks[i].upperPoint_.z_ - lowerBricks[i].lowerPoint_.z_);
				topsOfBricks_[x][y].brickIndex = i;
			}
		}
		std::vector<int> lowerIndexesVec;
		std::copy(brickIndexes.begin(), brickIndexes.end(), std::back_inserter(lowerIndexesVec));
		lowerBricks[i].addLowerPoints(lowerIndexesVec);

		for (auto&& lowerIndex : lowerIndexesVec)
		{
			lowerBricks[lowerIndex].addUpperIndex(i);
		}
	}

	return lowerBricks;
}

long long Solution::solve(const InputType& input) const
{
	std::vector<Brick> lowerBricks = prepare(input);

	std::set<int> cannotBeMoveed;
	for (auto&& el : lowerBricks)
	{
		if (el.blocksLower_.size() == 1)
		{
			cannotBeMoveed.insert(el.blocksLower_[0]);
		}
	}
	
	return lowerBricks.size() - cannotBeMoveed.size();
}

// 20271 to low
std::vector<std::pair <int, int>> toRestore;

long long checkDropChain(int brickIndex, std::vector<Brick>& allBricks)
{
	auto sum = 0ll;
	for (auto upperIndex : allBricks[brickIndex].blocksUpper_)
	{
		auto it = std::ranges::find(allBricks[upperIndex].blocksLower_, brickIndex);
		if (it != std::end(allBricks[upperIndex].blocksLower_))
		{
			toRestore.emplace_back(upperIndex, *it);
			allBricks[upperIndex].blocksLower_.erase(it);
			if (allBricks[upperIndex].blocksLower_.empty())
			{
				sum += 1;
				sum += checkDropChain(upperIndex, allBricks);
			}
		}
	}

	return sum;
}

long long Solution::solve_part2(const InputType& input) const
{
	std::vector<Brick> lowerBricks = prepare(input);

	auto sum = 0ll;
	for (int i = 0; i < lowerBricks.size(); ++i)
	{
		toRestore = {};
		sum += checkDropChain(i, lowerBricks);

		for (auto&& [upperIndex, index] : toRestore)
		{
			lowerBricks[upperIndex].blocksLower_.push_back(index);
		}
	}

	return sum;
}

}  // namespace year_2023::day22
