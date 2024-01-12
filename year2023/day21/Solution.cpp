#include "Solution.hpp"

#include <algorithm>
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
	auto pos = std::find(input.begin(), input.end(), 'S');
	if (pos != input.end())
	{
		return pos.getPoint();
	}
	return PointRowCol{};
}

auto countOs(const InputType& input)
{
	return std::count_if(input.begin(), input.end(), [](auto c) { return c == 'O'; });
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

			if (visitedNodes[i][node] == 'O' || visitedNodes[i][node] == '#')
			{
				continue;
			}
			visitedNodes[i][node] = 'O';
			for (auto&& point : getInBoundsNeighbours(input, node))
			{
				toVisitNextIteration.push(point);
			}
		}
	}

	return countOs(visitedNodes.back());
}

long long Solution::solve_part2(InputType) const
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