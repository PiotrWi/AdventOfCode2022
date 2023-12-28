#include "Solution.hpp"

#include <ranges>
#include <queue>
#include <iostream>

#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>

#include <utility/PointRowCol.hpp>

namespace year_2023::day21
{

InputType parse()
{
	return parsers::getFile(2023, 21) | To<InputType>();
}

auto findS(const InputType& input)
{
	for (auto row = 0; row < input.size(); ++row)
	{
		auto sPos = input[row].find('S');
		if (sPos != std::string::npos)
		{
			return PointRowCol{ row, (int)sPos };
		}
	}
	return PointRowCol{};
}

auto countOs(const InputType& input)
{
	auto sum = 0ll;
	for (auto row = 0; row < input.size(); ++row)
	{
		sum += std::ranges::count_if(input[row], [](auto c) { return c == 'O'; });
	}
	return sum;
}

long long Solution::solve(const InputType& input) const
{
	std::vector<InputType> visitedNodes;
	std::queue<PointRowCol> toVisitPoints;
	std::queue<PointRowCol> toVisitNextIteration;

	auto startingPoint = findS(input);
	toVisitNextIteration.push(startingPoint);


	for (int i = 0; i <= 64; ++i)
	{
		visitedNodes.push_back(input);
		toVisitPoints = toVisitNextIteration;
		toVisitNextIteration = {};

		while (not toVisitPoints.empty())
		{
			auto node = toVisitPoints.front();
			toVisitPoints.pop();

			if (visitedNodes[i][node.row][node.col] == 'O' || visitedNodes[i][node.row][node.col] == '#')
			{
				continue;
			}
			visitedNodes[i][node.row][node.col] = 'O';
			for (auto&& diff : { UpperPointDiff, BottomPointDiff, RightPointDiff, LeftPointDiff })
			{
				auto neighbour = node + diff;
				if (inBounds(neighbour, input.size(), input[0].size()))
				{
					toVisitNextIteration.push(neighbour);
				}
			}
		}
	}

	return countOs(visitedNodes.back());
}

long long Solution::solve_part2(InputType input) const
{
	auto n = (26501365 - 65) / 131;
	long long pn = 3941;
	long long pp = 3853;
	long long rest = 15642;

	std::vector<long long> pn_multipliers {1, 4};
	std::vector<long long> pp_multipliers {0, 1};
	std::vector<long long> rest_multipliers {0, 1};

	for (int i = 2; i <= n; ++i)
	{
		pp_multipliers.push_back(pn_multipliers.back());
		pn_multipliers.push_back(pn_multipliers[i - 2] + 4 * i);
		rest_multipliers.push_back(rest_multipliers.back() + i);
	}


	return pn * pn_multipliers.back() + pp * pp_multipliers.back() + rest * rest_multipliers.back();
}

}  // namespace year_2023::day21