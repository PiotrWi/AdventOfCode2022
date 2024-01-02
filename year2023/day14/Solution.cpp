#include "Solution.hpp"

#include <map>
#include <ranges>
#include <algorithm>
#include <iostream>

#include <parsers/parsers.hpp>
#include <utility/RangesUtils.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day14
{

InputType parse()
{
	return parsers::getFile(2023, 14) | To<InputType>{};
}

namespace
{

void performRockToLeft(InputType& input)
{
	for (auto& line : input)
	{
		auto groups = s_view::splitGroups(line, [](char c) { return c == '.' || c == 'O'; });
		for (auto&& group : groups)
		{
			std::sort(group.rbegin(), group.rend());
		}
	}
}

void performRockToRight(InputType& input)
{
	for (auto& line : input)
	{
		auto groups = s_view::splitGroups(line, [](char c) { return c == '.' || c == 'O'; });
		for (auto&& group : groups)
		{
			std::sort(group.begin(), group.end());
		}
	}
}

auto prerformCycleNorth(const InputType& input)
{
	auto transpozedInput = transpose(input);
	performRockToLeft(transpozedInput);
	return transpose(transpozedInput);
}

auto prerformCycleWest(const InputType& input)
{
	auto out = input;
	performRockToLeft(out);
	return out;
}

auto prerformCycleSouth(const InputType& input)
{
	auto transpozedInput = transpose(input);
	performRockToRight(transpozedInput);
	return transpose(transpozedInput);
}

auto prerformCycleEast(const InputType& input)
{
	auto out = input;
	performRockToRight(out);
	return out;
}

auto calculate(const InputType& input)
{
	auto transpozedInput = transpose(input);

	auto sum = 0ll;
	for (auto line : transpozedInput)
	{
		for (auto i = 0u; i < line.size(); ++i)
		{
			if (line[i] == 'O')
			{
				sum += line.size() - i;
			}
		}
	}
	return sum;
}

struct CycleFinder
{
	void add(const InputType& input, long long solution)
	{
		for (auto i = 0u; i < data_.size(); ++i)
		{
			if (input == data_[i])
			{
				cycle_ = data_.size() - i;
				solutons_[data_.size() % *cycle_] = solution;
				data_.push_back(input);
				return;
			}
		}
		data_.push_back(input);
	}
	bool ready()
	{
		return cycle_ && solutons_.size() == *cycle_;
	}
	std::optional<unsigned int> cycle_;
	std::vector<InputType> data_;
	std::map<int, long long> solutons_;
};

}  // namespace 

long long Solution::solve(const InputType& input) const
{
	auto out = prerformCycleNorth(input);
	return calculate(out);
}

long long Solution::solve_part2(const InputType& input) const
{
	CycleFinder c;
	auto out = input;
	for (auto i = 0ll; i < 1000'000'000ll; ++i)
	{
		c.add(out, calculate(out));
		if (c.ready())
		{
			break;
		}
		out = prerformCycleNorth(out);
		out = prerformCycleWest(out);
		out = prerformCycleSouth(out);
		out = prerformCycleEast(out);

	}
	return c.solutons_[1000'000'000ll % *c.cycle_];
}

}  // namespace year_2023::day14
