#include "Solution.hpp"

#include <optional>
#include <set>
#include <map>
#include <iostream>
#include <ranges>
#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>
#include <utility/PointRowCol.hpp>

namespace year_2023::day23
{

InputType parse()
{
	return parsers::getFile(2023, 23) | To<InputType>();
}

auto findAllCrossings(const InputType& input, PointRowCol startingPoint, PointRowCol finishPoint)
{
	std::vector <PointRowCol> crossings;
	crossings.push_back(startingPoint);
	crossings.push_back(finishPoint);

	for (auto row = 0u; row < input.size(); ++row)
	{
		for (auto col = 0u; col < input.size(); ++col)
		{
			auto visitedPoint = PointRowCol{ (int)row, (int)col };
			if (input[visitedPoint.row][visitedPoint.col] == '#')
			{
				continue;
			}
			auto emtpyNeignours = 0;
			for (auto diff : { BottomPointDiff, UpperPointDiff, LeftPointDiff, RightPointDiff })
			{
				auto neighbour = visitedPoint + diff;

				if (inBounds(neighbour, input.size(), input[0].size()) && input[neighbour.row][neighbour.col] != '#')
				{
					emtpyNeignours += 1;
				}
			}
			if (emtpyNeignours > 2)
			{
				crossings.push_back({ (int)row, (int)col });
			}
		}
	}
	return crossings;
}

struct Node;

struct Neighbour
{
	PointRowCol coordinates;
	int cost;
	Node* node;
};

struct Node
{
	PointRowCol begin;
	int index;
	std::vector<Neighbour> neighbours;
};

bool allowedToPass(const InputType& input, PointRowCol previousPoint, PointRowCol currentPoint)
{
	return input[previousPoint.row][previousPoint.col] == '.' ||
		(input[previousPoint.row][previousPoint.col] == '>' && previousPoint + RightPointDiff == currentPoint) ||
		(input[previousPoint.row][previousPoint.col] == '<' && previousPoint + LeftPointDiff == currentPoint) ||
		(input[previousPoint.row][previousPoint.col] == 'v' && previousPoint + BottomPointDiff == currentPoint) ||
		(input[previousPoint.row][previousPoint.col] == '^' && previousPoint + UpperPointDiff == currentPoint);
}

template <bool TUseSlopes>
std::optional<Neighbour> followTheRoad(const InputType& input, const std::vector <PointRowCol>& crossings, PointRowCol previousPoint, PointRowCol currentPoint, int cost)
{
	if (not inBounds(currentPoint, input.size(), input[0].size())
		|| input[currentPoint.row][currentPoint.col] == '#')
	{
		return {};
	}

	if (TUseSlopes && not allowedToPass(input, previousPoint, currentPoint))
	{
		return {};
	}

	if (std::ranges::find(crossings, currentPoint) != crossings.end())
	{
		return Neighbour{ currentPoint, cost, {} };
	}

	for (auto diff : { BottomPointDiff, UpperPointDiff, LeftPointDiff, RightPointDiff })
	{
		auto nextPoint = currentPoint + diff;
		if (nextPoint != previousPoint)
		{
			auto out = followTheRoad<TUseSlopes>(input, crossings, currentPoint, nextPoint, cost + 1);
			if (out)
			{
				return out;
			}
		}
	}
	return {};
}

template <bool TUseSlopes>
auto fullfillNodes(const InputType& input, const std::vector <PointRowCol>& crossings)
{
	std::vector <Node> nodes;

	for (auto&& crossing: crossings)
	{
		Node node;
		node.begin = crossing;
		for (auto diff : { BottomPointDiff, UpperPointDiff, LeftPointDiff, RightPointDiff })
		{
			auto neighbourOpt = followTheRoad<TUseSlopes>(input, crossings, crossing, crossing + diff, 1);
			if (neighbourOpt)
			{
				node.neighbours.push_back(*neighbourOpt);
			}
		}
		nodes.push_back(node);
	}

	int i = 0;
	for (auto& node : nodes)
	{
		node.index = i++;
		for (auto& neigbour : node.neighbours)
		{
			neigbour.node = &nodes[std::ranges::find_if(nodes, [&](auto elem) { return neigbour.coordinates == elem.begin;  }) - nodes.begin()];
		}
	}

	return nodes;
}

int max = 0;

void visit(Node* currentNode, PointRowCol finishPoint, unsigned long long visited, int cost)
{
	if ((1ull << currentNode->index) & visited)
	{
		return;
	}
	visited |= (1ull << currentNode->index);

	if (currentNode->begin == finishPoint)
	{
		max = std::max(cost, max);
	}

	for (auto neigbour : currentNode->neighbours)
	{
		visit(neigbour.node, finishPoint, visited, cost + neigbour.cost);
	}
}

 
long long Solution::solve(const InputType& input) const
{
	max = 0;
	auto startingPoint = PointRowCol{ 0, 1 };
	auto finishPoint = PointRowCol{ (int)input.size() - 1, (int)input[0].size() - 2};
	std::vector <PointRowCol> crossings = findAllCrossings(input, startingPoint, finishPoint);
	std::vector <Node> nodes = fullfillNodes<true>(input, crossings);

	visit(&nodes[0], finishPoint, 0ull, 1);

	return max;
}


long long Solution::solve_part2(const InputType& input) const
{
	max = 0;
	auto startingPoint = PointRowCol{ 0, 1 };
	auto finishPoint = PointRowCol{ (int)input.size() - 1, (int)input[0].size() - 2 };
	std::vector <PointRowCol> crossings = findAllCrossings(input, startingPoint, finishPoint);
	std::vector <Node> nodes = fullfillNodes<false>(input, crossings);

	visit(&nodes[0], finishPoint, 0ull, 1);

	return max;
}

}  // namespace year_2023::day23
