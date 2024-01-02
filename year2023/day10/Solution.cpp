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

struct Pos
{
	int row = 0;
	int col = 0;
};

Pos operator+ (const Pos& lhs, const Pos& rhs)
{
	Pos p;
	p.row = lhs.row + rhs.row;
	p.col = lhs.col + rhs.col;
	return p;
}

template <typename T>
auto isInBound(int row, int collumn, const std::vector< std::vector<T> >& nodes)
{
	return row >= 0 && row < (int)nodes.size() && collumn >= 0 && collumn < (int)nodes[0].size();
}

void addNeighbour(Node& node, int row, int collumn, std::vector< std::vector<Node> >& nodes)
{
	if (not isInBound(row, collumn, nodes))
	{
		return;
	}
	node.neighbours.push_back(&nodes[row][collumn]);
}

}  // namespace

InputType parse()
{
	InputType input;
	for (auto&& line : parsers::getFile(2023, 10))
	{
		input.nodes.emplace_back(line | std::views::transform([](auto&& c) { return Node(c); }) | ToVector{});
	}

	auto add = std::bind(addNeighbour, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::ref(input.nodes));
	for (int row = 0; row < (int)input.nodes.size(); ++row)
	{
		for (int col = 0; col < (int)input.nodes[row].size(); ++col)
		{
			auto& node = input.nodes[row][col];
			switch (node.symbol)
			{
			case '|':
				add(node, row + 1, col);
				add(node, row - 1, col);
				break;
			case '-':
				add(node, row, col - 1);
				add(node, row, col + 1);
				break;
			case 'L':
				add(node, row - 1, col);
				add(node, row, col + 1);
				break;
			case 'J':
				add(node, row - 1, col);
				add(node, row, col - 1);
				break;
			case '7':
				add(node, row + 1, col);
				add(node, row, col - 1);
				break;
			case 'F':
				add(node, row + 1, col);
				add(node, row, col + 1);
				break;
			case 'S':
				add(node, row + 1 , col);
				add(node, row, col - 1);
				input.startingRow = row;
				input.startingCol = col;
				node.symbol = '7';
				break;
			case '.':
				break;
			default:
				throw 1;
				break;	
			}
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
	auto& startingNode = input.nodes[input.startingRow][input.startingCol];
	startingNode.distanceFromStart = 0;
	for (auto& neigbourNode : input.nodes[input.startingRow][input.startingCol].neighbours)
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
	for (auto row = 0u; row < input.nodes.size(); ++row)
	{
		for (auto col = 0u; col < input.nodes[row].size(); ++col)
		{
			auto& node = input.nodes[row][col];
			if (not node.distanceFromStart)
			{
				node.symbol = '.';
			}
		}
	}

	auto fields = std::vector(input.nodes.size() * 3, std::vector <char>(input.nodes[0].size() * 3, '.'));
	auto enqueued = std::vector(input.nodes.size() * 3, std::vector <bool>(input.nodes[0].size() * 3, false));
	auto queue = std::queue<Pos>();

	for (auto row = 0u; row < input.nodes.size(); ++row)
	{
		for (auto col = 0u; col < input.nodes[row].size(); ++col)
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
	queue.push(Pos{ 0, 0 });

	while (not queue.empty())
	{
		auto pos = queue.front();
		queue.pop();
		auto& field = fields[pos.row][pos.col];
		if (field == '.')
		{
			field = 'O';
			for (auto&& diff : { Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1} })
			{
				auto nextPos = pos + diff;
				if (not isInBound(nextPos.row, nextPos.col, fields) || enqueued[nextPos.row][nextPos.col])
				{
					continue;
				}
				queue.push(nextPos);
				enqueued[nextPos.row][nextPos.col] = true;
			}
		}

	}

	auto innerFields = 0ll;
	for (auto row = 0u; row < input.nodes.size(); ++row)
	{
		for (auto col = 0u; col < input.nodes[row].size(); ++col)
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
