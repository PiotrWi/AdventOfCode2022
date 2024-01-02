#include "Solution.hpp"

#include <iostream>
#include <tuple>
#include <ranges>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day15
{

InputType parse()
{
	auto file = parsers::getFile(2023, 15);
	auto line = *file.begin();
	return splitAndTrim(line, ',');
}

namespace
{

auto extract(const std::string& word, char separator)
{
	std::string label;
	std::optional<int> focalLen;

	auto labelToFocal = splitAndTrim(word, separator);
	label = labelToFocal[0];
	if (labelToFocal.size() > 1)
	{
		focalLen = std::stoi(labelToFocal[1]);
	}

	return std::make_tuple(label, focalLen);
}

unsigned char calculateBox(const std::string& word)
{
	unsigned char val = 0;
	for (unsigned char c : word)
	{
		val = (val + c) * 17;
	}
	return val;
}

}  // namespace

long long Solution::solve(const InputType& input) const
{
	long long out = 0;
	for (auto&& word : input)
	{
		out += calculateBox(word);
	}
	return out;
}

struct BoxEntity
{
	std::string label;
	int focalLen;
};

long long Solution::solve_part2(const InputType& input) const
{
	std::vector<BoxEntity> boxes[256];
	for (auto&& word : input)
	{
		auto hasMinus = word.find('-') != std::string::npos;
		auto separator = hasMinus ? '-': '=';
		auto [label, focalLen] = extract(word, separator);

		auto box = calculateBox(label);
		auto labelPred = [label](auto&& el) { return el.label == label; };

		if (hasMinus)
		{
			boxes[box].erase(std::remove_if(boxes[box].begin(), boxes[box].end(), labelPred), boxes[box].end());
		}
		else
		{
			auto it = std::ranges::find_if(boxes[box], labelPred);
			if (it != boxes[box].end())
			{
				it->focalLen = *focalLen;
			}
			else
			{
				boxes[box].push_back({ label, *focalLen });
			}
		}
	}

	long long out = 0ll;
	for (auto boxNr = 0u; boxNr < 256u; ++boxNr)
	{
		for (auto pos = 0u; pos < boxes[boxNr].size(); ++pos)
		{
			out += (boxNr + 1) * (pos + 1) * boxes[boxNr][pos].focalLen;
		}
	}
	return out;
}

}  // namespace year_2023::day15