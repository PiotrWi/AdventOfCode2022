#include "Solution.hpp"

#include <functional>
#include <ranges>
#include <queue>

#include "parsers/parsers.hpp"
#include <utility/RangesUtils.hpp>

namespace year_2023::day10
{

namespace
{

void addNeighbour(Node& node, PointRowCol point, Matrix<Node>& nodes)
{
	if (not nodes.inBounds(point))
	{
		return;
	}
	node.neighbours.push_back(&nodes[point]);
}

}  // namespace

InputType parse()
{
	InputType input;
	for (auto&& line : parsers::getFile(2023, 10))
	{
		input.nodes.push_back(line | std::views::transform([](auto&& c) { return Node(c); }) | ToVector{});
	}

	auto add = std::bind(addNeighbour, std::placeholders::_1, std::placeholders::_2, std::ref(input.nodes));
	for (auto it = input.nodes.begin(); it != input.nodes.end(); ++it)
	{
			auto& node = *it;
			auto point = it.getPoint();
			switch (node.symbol)
			{
			case '|':
				add(node, point + BottomPointDiff);
				add(node, point + UpperPointDiff);
				break;
			case '-':
				add(node, point + LeftPointDiff);
				add(node, point + RightPointDiff);
				break;
			case 'L':
				add(node, point + UpperPointDiff);
				add(node, point + RightPointDiff);
				break;
			case 'J':
				add(node, point + UpperPointDiff);
				add(node, point + LeftPointDiff);
				break;
			case '7':
				add(node, point + BottomPointDiff);
				add(node, point + LeftPointDiff);
				break;
			case 'F':
				add(node, point + BottomPointDiff);
				add(node, point + RightPointDiff);
				break;
			case 'S':
				add(node, point + BottomPointDiff);
				add(node, point + LeftPointDiff);
				input.startingPoint = point;
				node.symbol = '7';
				break;
			case '.':
				break;
			default:
				throw 1;
				break;	
			}
	}
	return input;
}

std::optional<long long> evaluatePipeForward(auto toVisit, int distance)
{
	while (true)
	{
		std::vector<Node*> neighbours;
		for (auto* visited : toVisit)
		{
			visited->distanceFromStart = distance;
			for (auto* neighbour : visited->neighbours)
			{
				if (distance != 1 && neighbour->distanceFromStart && neighbour->distanceFromStart == 0)
				{
					return (distance + 1ll) / 2;
				}
				if (not neighbour->distanceFromStart)
				{
					neighbours.push_back(neighbour);
				}
			}			
		}
		distance += 1;
		toVisit = neighbours;
	}
}

std::optional<long long> evaluatePipe(InputType& input)
{
	auto& startingNode = input.nodes[input.startingPoint];
	startingNode.distanceFromStart = 0;
	for (auto& neigbourNode : input.nodes[input.startingPoint].neighbours)
	{
		std::vector<Node*> toVisit;
		toVisit.emplace_back(neigbourNode);
		if (auto res = evaluatePipeForward(toVisit, 1))
		{
			return *res;
		}
	}
	return {};
}

long long Solution::solve(InputType& input) const
{
	return *evaluatePipe(input);
}

long long Solution::solve_part2(InputType& input) const
{
	for (auto& node: input.nodes)
	{
		if (not node.distanceFromStart)
		{
			node.symbol = '.';
		}
	}

	auto fields = Matrix<char>(input.nodes.rows_count() * 3, input.nodes.cols_count() * 3, '.');
	auto enqueued = std::vector<std::vector<bool>>(input.nodes.rows_count() * 3, std::vector<bool>(input.nodes.cols_count() * 3, false));
	auto queue = std::queue<PointRowCol>();

	for (auto row = 0u; row < input.nodes.rows_count(); ++row)
	{
		for (auto col = 0u; col < input.nodes.cols_count(); ++col)
		{
			auto& node = input.nodes[row][col];
			switch (node.symbol)
			{
			case '|':
				fields[row * 3][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 2][col * 3 + 1] = 'X';
				break;
			case '-':
				fields[row * 3 + 1][col * 3] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3 + 2] = 'X';
				break;
			case 'L':
				fields[row * 3][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3 + 2] = 'X';
				break;
			case 'J':
				fields[row * 3][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 1][col * 3] = 'X';
				break;
			case '7':
				fields[row * 3 + 1][col * 3] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 2][col * 3 + 1] = 'X';
				break;
			case 'F':
				fields[row * 3 + 1][col * 3 + 2] = 'X';
				fields[row * 3 + 1][col * 3 + 1] = 'X';
				fields[row * 3 + 2][col * 3 + 1] = 'X';
				break;
			case '.':
				break;
			default:
				throw 1;
				break;
			}
		}
	}

	enqueued[0][0] = true;
	queue.push(PointRowCol{ 0, 0 });

	while (not queue.empty())
	{
		auto pos = queue.front();
		queue.pop();
		auto& field = fields[pos];
		if (field == '.')
		{
			field = 'O';
			for (auto&& nextPos : getInBoundsNeighbours(fields, pos))
			{
				if (enqueued[nextPos.row][nextPos.col])
				{
					continue;
				}
				queue.push(nextPos);
				enqueued[nextPos.row][nextPos.col] = true;
			}
		}

	}

	auto innerFields = 0ll;
	for (auto row = 0u; row < input.nodes.rows_count(); ++row)
	{
		for (auto col = 0u; col < input.nodes.cols_count(); ++col)
		{
			auto isInner =
				fields[row * 3][col * 3] == '.' && fields[row * 3][col * 3 + 1] == '.' && fields[row * 3][col * 3 + 2] == '.' &&
				fields[row * 3 + 1][col * 3] == '.' && fields[row * 3 + 1][col * 3 + 1] == '.' && fields[row * 3 + 1][col * 3 + 2] == '.' &&
				fields[row * 3 + 2][col * 3] == '.' && fields[row * 3 + 2][col * 3 + 1] == '.' && fields[row * 3 + 2][col * 3 + 2] == '.';
			if (isInner)
			{
				innerFields += 1;
			}
		}
	}

	return innerFields;
}

}  // namespace year_2023::day10
