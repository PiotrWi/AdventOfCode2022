#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <map>
#include <iterator>
#include <iostream>

#include <utility/RangesUtils.hpp>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day8
{

const char* fileLoc = "year2023/day8/input.txt";

Input parse()
{
	Input input;
	parsers::LinesInFileRange file(fileLoc);

	auto fileIt = file.begin();
	auto firstLine = *fileIt;
	input.sides = firstLine | std::views::transform([](auto&& c) { if (c == 'L') return Side::left; return Side::Right; }) | ToVector{};

	++fileIt;

	for (; fileIt != file.end(); ++fileIt)
	{
		SingleNode sn;
		auto& line = *fileIt;
		if (line.empty()) continue;
		auto idToNeighbours = splitAndTrim(line, '=');
		sn.id = idToNeighbours[0];

		auto neigbourts = splitAndTrim(idToNeighbours[1].substr(1, idToNeighbours[1].size() - 2), ',');
		sn.neighboursIds[0] = neigbourts[0];
		sn.neighboursIds[1] = neigbourts[1];
		input.nodes.push_back(sn);
	}

	std::map<std::string, SingleNode*> IdToNodePtr;
	for (auto& node : input.nodes)
	{
		IdToNodePtr[node.id] = &node;
	}
	for (auto& node : input.nodes)
	{
		node.getLeftNode() = IdToNodePtr[node.getLeftId()];
		node.getRightNode() = IdToNodePtr[node.getRightId()];

	}
	return input;
}

class SideCyclicBufor
{
public:
	SideCyclicBufor(std::vector<Side>& sides)
		: current_(sides.begin())
		, begin_(sides.begin())
		, end_ (sides.end())
	{}
	Side getNext()
	{
		if (current_ == end_)
		{
			current_ = begin_;
		}
		return *current_++;
	}
private:
	std::vector<Side>::iterator current_;
	std::vector<Side>::iterator begin_;
	std::vector<Side>::iterator end_;
};

long long Solution::solve(Input& input) const
{
	long long steps = 0;
	SideCyclicBufor sideGenerator_(input.sides);

	SingleNode* node = &(*std::find_if(input.nodes.begin(), input.nodes.end(), [](auto&& el) { return el.id == "AAA"; }));
	while (node->id != "ZZZ")
	{
		node = node->neighboursNodes[static_cast<int>(sideGenerator_.getNext())];
		++steps;
	}
	return steps;
}

namespace
{

class CycleFinder
{
public:
	void add(long long steps, std::string id)
	{
		if (not id_.empty() && id != id_)
		{
			throw 1;
		}
		id_ = id;
		if (not steps_.empty())
		{
			diffs_.push_back(steps - steps_.back());
			if (diffs_.back() != diffs_.front())
			{
				throw 2;
			}
		}
		steps_.push_back(steps);
	}
	bool isComplete() const
	{
		return diffs_.size();
	}

	// Altouht, distance from starting point to **Z does not need to be in cycle, here it is.
	long long getDiff() const
	{
		return diffs_.front();
	}
private:
	std::string id_;
	std::vector<long long> steps_;
	std::vector<long long> diffs_;
};

}

long long Solution::solve_part2(Input& input) const
{
	long long steps = 0;

	SideCyclicBufor sideGenerator_(input.sides);
	CycleFinder cycles[6];

	std::vector<SingleNode*> currentNodes;
	auto currentNodesRange = input.nodes
		| std::views::filter([](auto&& node) { return node.id[2] == 'A'; })
		| std::views::transform([](auto&& el) { return &el; });
	std::copy(currentNodesRange.begin(), currentNodesRange.end(), std::back_inserter(currentNodes));

	while (std::ranges::any_of(cycles, [](auto&& el) { return not el.isComplete(); }))
	{
		for (int i = 0; i < 6; ++i)
		{
			if (currentNodes[i]->id[2] == 'Z')
			{
				cycles[i].add(steps, currentNodes[i]->id);
			}
		}
		auto sideToGo = static_cast<int>(sideGenerator_.getNext());
		for (auto*& node : currentNodes)
		{
			node = node->neighboursNodes[sideToGo];
		}
		++steps;
	}

	steps = input.sides.size();
	for (auto&& cycle : cycles)
	{
		steps *= (cycle.getDiff() / input.sides.size());
	}
	return steps;
}

} // namespace year_2023::day8
