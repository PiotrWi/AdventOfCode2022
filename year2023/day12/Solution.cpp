#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <iostream>
#include <unordered_map>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day12
{

InputType parse()
{
	InputType in;
	for (auto&& inputLine : parsers::getFile(2023, 12))
	{
		Line l;
		auto patternToList = splitAndTrim(inputLine, ' ');
		l.line = patternToList[0];
		l.groups = splitAndTrim(patternToList[1], ',') | std::views::transform([](auto&& el) { return std::stoi(el);  }) | ToVector{};
		in.push_back(l);
	}
	return in;
}

namespace
{

struct PairHash
{
	std::size_t operator()(const std::pair<int, int>& p) const noexcept
	{
		return p.first * (1ull << 32) + p.second;
	}
};
std::unordered_map<std::pair<int, int>, long long, PairHash> cache;

bool canBeMatch(int position, const std::string& line, int len)
{
	return line.size() >= position + len
		&& std::all_of(line.data() + position, line.data() + position + len, [](char c) { return c == '#' || c == '?'; })
		&& ((line.size() > position + len)
			? line[position + len] != '#'
			: true);
			
}

long long evaluate(int position, std::string& line, const std::vector<int>& groups, int groupToMatch)
{
	if (position >= line.size())
	{
		return groupToMatch == groups.size();
	}
	if (line[position] == '#')
	{
		if (groupToMatch >= groups.size())
		{
			return 0;
		}
		auto len = groups[groupToMatch];
		if (not canBeMatch(position, line, len))
		{
			return 0;
		}
		position += len + 1;

		return evaluate(position, line, groups, groupToMatch + 1);
	}
	if (line[position] == '.')
	{
		if (auto node = cache.find({position, groupToMatch}) != cache.end())
		{
			return cache.find({ position, groupToMatch })->second;
		}
		auto solution = evaluate(position + 1, line, groups, groupToMatch);
		cache.insert({ { position, groupToMatch }, solution});
		return solution;
	}
	auto line1 = line;
	line1[position] = '.';
	line[position] = '#';
	return evaluate(position, line1, groups, groupToMatch) + evaluate(position, line, groups, groupToMatch);
}

}  // namespace

long long Solution::solve(const InputType& input) const
{
	auto sum = 0ll;
	for (auto&& entity : input)
	{
		cache = {};
		auto line = entity.line;
		sum += evaluate(0, line, entity.groups, 0);
	}
	return sum;
}

long long Solution::solve_part2(const InputType& input) const
{
	auto i = 0;
	auto sum = 0ll;
	for (auto&& entity : input)
	{
		cache = {};
		Line entity2;
		for (int i = 0; i < 5; ++i)
		{
			entity2.line += entity.line;
			if (i != 4) { entity2.line += '?'; }
			std::copy(entity.groups.begin(), entity.groups.end(), std::back_inserter(entity2.groups));
		}
		auto line = entity2.line;
		auto res = evaluate(0, line, entity2.groups, 0);
		sum += res;
	}
	return sum;
}

}  // namespace year_2023::day12
