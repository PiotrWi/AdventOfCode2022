#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <queue>

#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>
#include <utility/PointRowCol.hpp>

namespace year_2023::day17
{

InputType parse()
{
	InputType in;
	for (auto&& line : parsers::getFile(2023, 17))
	{
		in.push_back(line
			| std::views::transform([](auto c) { return c - '0'; })
			| To<std::vector<int>>());
	}
	return in;
}

namespace
{

enum class Direction
{
	horizontal,
	vertical,
};

struct NodeToVisit
{
	int cost;
	Direction dirrection;
	PointRowCol point;
};

bool operator< (const NodeToVisit& lhs, const NodeToVisit& rhs)
{
	return lhs.cost > rhs.cost;
}

auto getShortestPathLenght(const InputType& input, int minStep, int maxStep)
{
	std::priority_queue<NodeToVisit> queue;
	Matrix<long long> minimalCostsFromHorizontal(input.rows_count(), input.cols_count(), 1000'000'000ll);
	Matrix<long long> minimalCostsFromVertical(input.rows_count(), input.cols_count(), 1000'000'000ll);

	queue.push(NodeToVisit{ 0, Direction::horizontal, {0, 0} });
	queue.push(NodeToVisit{ 0, Direction::vertical, {0, 0} });

	while (not queue.empty())
	{
		auto node = queue.top();
		queue.pop();
		if (node.dirrection == Direction::horizontal)
		{
			auto point = node.point;
			if (minimalCostsFromVertical[point] <= node.cost)
			{
				continue;
			}
			minimalCostsFromVertical[point] = node.cost;

			for (auto&& diff : { LeftPointDiff, RightPointDiff })
			{
				NodeToVisit nextNode = node;
				nextNode.dirrection = Direction::vertical;

				for (int i = 1; i <= maxStep; ++i)
				{
					nextNode.point = nextNode.point + diff;
					if (not input.inBounds(nextNode.point))
					{
						continue;
					}
					nextNode.cost = nextNode.cost + input[nextNode.point];
					if (i >= minStep)
					{
						queue.push(nextNode);
					}
					
				}
			}
		}
		else
		{
			auto point = node.point;
			if (minimalCostsFromHorizontal[point] <= node.cost)
			{
				continue;
			}
			minimalCostsFromHorizontal[point] = node.cost;

			for (auto&& diff : { BottomPointDiff, UpperPointDiff })
			{
				NodeToVisit nextNode = node;
				nextNode.dirrection = Direction::horizontal;

				for (int i = 1; i <= maxStep; ++i)
				{
					nextNode.point = nextNode.point + diff;
					if (not input.inBounds(nextNode.point))
					{
						continue;
					}
					nextNode.cost = nextNode.cost + input[nextNode.point];
					if (i >= minStep)
					{
						queue.push(nextNode);
					}
				}
			}
		}
	}

	return std::min(minimalCostsFromHorizontal.back(), minimalCostsFromVertical.back());
}

} // namespace

long long Solution::solve(const InputType& input) const
{
	return getShortestPathLenght(input, 1, 3);
}

// 761 to low
long long Solution::solve_part2(const InputType& input) const
{
	return getShortestPathLenght(input, 4, 10);
}


}  // namespace year_2023::day17
