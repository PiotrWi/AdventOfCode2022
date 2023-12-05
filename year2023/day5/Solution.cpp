#include "Solution.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <tuple>

#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day5
{

bool operator< (const Ranges& lhs, const Ranges& rhs)
{
	return lhs.sourceBegin < rhs.sourceBegin;
}

namespace
{

const long long BigNumber = 1000000000000ll;

void prepare(Context& ctx)
{
	for (auto& range : ctx.ranges)
	{
		std::sort(range.begin(), range.end());
	}
}

auto mapTo(long long in, int rangeIndex, long long rangeTail, const std::vector< std::vector<Ranges> >& rangeMappings)
{
	if (rangeIndex >= rangeMappings.size())
	{
		return std::make_tuple(in, rangeTail);
	}

	for (auto it = rangeMappings[rangeIndex].begin(); it != rangeMappings[rangeIndex].end(); ++it)
	{
		auto& singleRange = *it;
		if (in >= singleRange.sourceBegin && in < singleRange.sourceBegin + singleRange.length)
		{
			auto dif = in - singleRange.sourceBegin;
			auto tail = std::min(rangeTail, singleRange.length - dif);
			return mapTo(singleRange.destinationBegin + dif, rangeIndex + 1, tail, rangeMappings);
		}

		if ((it + 1) != rangeMappings[rangeIndex].end() && (it + 1)->sourceBegin > in)
		{
			auto thisTail = (it + 1)->sourceBegin - in;
			auto tail = std::min(rangeTail, thisTail);
			return mapTo(in, rangeIndex + 1, tail, rangeMappings);
		}
	}

	return mapTo(in, rangeIndex + 1, BigNumber, rangeMappings);
}

}  // namespace

const char* fileLoc = "year2023/day5/input.txt";

Context parse()
{
	Context ctx;
	parsers::LinesInFileRange linesInFile(fileLoc);

	auto fileIt = linesInFile.begin();
	auto seedLine = *fileIt; ++fileIt;

	ctx.seeds = splitAndTrim(splitAndTrim(seedLine, ':')[1], ' ') | std::views::transform([](auto&& e) { return std::stoll(e); }) | ToVector{};

	std::vector<Ranges> ranges;
	while (fileIt != linesInFile.end())
	{
		auto& line = *fileIt;
		if (line.empty() && not ranges.empty())
		{
			ctx.ranges.push_back(ranges);
			ranges = {};
		}
		if (not line.empty() && std::isdigit(line[0]))
		{
			Ranges r{};
			std::stringstream ss(line);
			ss >> r.destinationBegin >> r.sourceBegin >> r.length;
			ranges.push_back(r);
		}

		++fileIt;
	}
	if (not ranges.empty())
	{
		ctx.ranges.push_back(ranges);
		ranges = {};
	}
	prepare(ctx);
	return ctx;
}

long long Solution::solve(const Context& input) const
{
	auto minLocation = BigNumber;
	for (auto&& seed : input.seeds)
	{
		auto [location, tail] = mapTo(seed, 0, 1, input.ranges);
		if (location < minLocation)
		{
			minLocation = location;
		}
	}
	return minLocation;
}

long long Solution::solve_part2(Context& input) const
{
	auto minLocation = BigNumber;
	for (auto i = 0; i < input.seeds.size(); i += 2)
	{
		for (auto j = 0ll; j < input.seeds[i + 1];)
		{
			auto seed = input.seeds[i] + j;
			auto [location, tail] = mapTo(seed, 0, input.seeds[i + 1] - j, input.ranges);
			if (location < minLocation)
			{
				minLocation = location;
			}
			j += tail;
		}
	}
	return minLocation;
}

}  // namespace year_2023::day5
