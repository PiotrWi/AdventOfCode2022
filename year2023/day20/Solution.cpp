#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <queue>
#include <utility/RangesUtils.hpp>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day20
{

struct Broadcaster : public INode
{
	explicit Broadcaster(std::vector<std::string> neighbours, std::string myLabel)
		: neighbours_(neighbours)
		, myLabel_(myLabel)
	{
	}
	void init() override {};
	std::vector<Signal> react(bool, std::string) override
	{
		return neighbours_
			| std::views::transform([&](auto&& str) { return Signal{ myLabel_, str, false }; })
			| To<std::vector<Signal>>();
	}
	void registerInput(std::string) override {}
	bool getState() const override { return false; };
	std::vector<std::string> getNeighbours() override
	{
		return neighbours_;
	}
private:
    std::vector<std::string> neighbours_;
	std::string myLabel_;
};

struct Conjunction : public INode
{
	explicit Conjunction(std::vector<std::string> neighbours, std::string myLabel)
		: neighbours_(neighbours)
		, myLabel_(myLabel)
	{
	}
	void init() override
	{
		for (auto&& state : statesOfInputs)
		{
			state.second = false;
		}
	};
	virtual void registerInput(std::string from) override
	{
		statesOfInputs[from] = false;
	}
	std::vector<Signal> react(bool signal, std::string from) override
	{
		statesOfInputs[from] = signal;
		auto stateToSend = not std::ranges::all_of(statesOfInputs, [](auto states) { return states.second; });

		return neighbours_
			| std::views::transform([&](auto&& str) { return Signal{ myLabel_, str, stateToSend }; })
			| To<std::vector<Signal>>();
	}
	bool getState() const override { return not std::ranges::all_of(statesOfInputs, [](auto states) { return states.second; }); };
	std::vector<std::string> getNeighbours() override
	{
		return neighbours_;
	}
private:
    std::vector<std::string> neighbours_;
    std::string myLabel_;
	std::map<std::string, bool> statesOfInputs;
};

struct FlipFlop : public INode
{
	explicit FlipFlop(std::vector<std::string> neighbours, std::string myLabel)
		: neighbours_(neighbours)
		, myLabel_(myLabel)
	{
	}
	void init() override { state_ = false; };
	std::vector<Signal> react(bool signal, std::string) override
	{
		if (signal == true)
		{
			return {};
		}
		state_ = !state_;
		return neighbours_
			| std::views::transform([this](auto&& str) { return Signal{ myLabel_, str, state_ }; })
			| To<std::vector<Signal>>();
	}
	void registerInput(std::string) override {}

	bool getState() const override { return state_; }
	std::vector<std::string> getNeighbours() override
	{
		return neighbours_;
	}
private:
    std::vector<std::string> neighbours_;
    std::string myLabel_;
    bool state_ = false;
};

InputType parse()
{
	InputType in;

	for (auto&& line : parsers::getFile(2023, 20))
	{
		auto labelsToNeighbours = splitAndTrimByStr(line, "->");
		auto neigbours = splitAndTrim(labelsToNeighbours[1], ',');

		if (labelsToNeighbours[0][0] == '%')
		{
			auto label = labelsToNeighbours[0].substr(1);
			in[label] = std::make_unique<FlipFlop>(neigbours, label);
		}
		else if (labelsToNeighbours[0][0] == '&')
		{
			auto label = labelsToNeighbours[0].substr(1);
			in[labelsToNeighbours[0].substr(1)] = std::make_unique<Conjunction>(neigbours, label);
		}
		else
		{
			in["broadcaster"] = std::make_unique<Broadcaster>(neigbours, "broadcaster");
		}
	}

	for (auto&& elem : in)
	{
		auto neighbours = elem.second->getNeighbours();
		for (auto neighbourStr : neighbours)
		{
			if (in.contains(neighbourStr))
			{
				in[neighbourStr]->registerInput(elem.first);
			}
		}
	}

	return in;
}

long long Solution::solve(InputType& input) const
{
	for (auto&& [labek, node] : input)
	{
		node->init();
	}
	auto lows = 0ull;
	auto highs = 0ull;

	for (int i = 0; i < 1000; ++i)
	{
		std::queue<Signal> queue; 
		queue.push({ "button", "broadcaster", false });
		lows += 1;
		while (not queue.empty())
		{
			auto signalData = queue.front();
			queue.pop();

			if (not input.contains(signalData.to))
			{
				continue;
			}
			auto nextSignals = input[signalData.to]->react(signalData.signal, signalData.from);
			lows += std::ranges::count_if(nextSignals, [](auto&& el) { return el.signal == false; });
			highs += std::ranges::count_if(nextSignals, [](auto&& el) { return el.signal == true; });

			for (auto&& next : nextSignals)
			{
				queue.push(next);
			}
		}
	}
	
	return lows * highs;
}

long long Solution::solve_part2(InputType& input) const
{
	std::vector<std::string> nodesToObserve { "mk", "fp", "zc", "xt"};
	std::map<std::string, long long> firstOcurences;

	for (auto&& [labek, node] : input)
	{
		node->init();
	}

	auto i = 1ll;
	while (firstOcurences.size() < 4)
	{
		std::queue<Signal> queue;
		queue.push({ "button", "broadcaster", false });
		while (not queue.empty())
		{
			auto signalData = queue.front();
			queue.pop();

			if (not input.contains(signalData.to))
			{
				continue;
			}
			auto nextSignals = input[signalData.to]->react(signalData.signal, signalData.from);

			for (auto&& next : nextSignals)
			{
				if (std::ranges::find(nodesToObserve, next.from) != nodesToObserve.end() && next.signal == true)
				{
					if (firstOcurences.count(next.from))
					{
						continue;
					}
					firstOcurences[next.from] = i;
				}
				queue.push(next);
			}
		}
		++i;
	}

	return firstOcurences["mk"] * firstOcurences["fp"] * firstOcurences["zc"] * firstOcurences["xt"];
}

}  // namespace year_2023::day20
