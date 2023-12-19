#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <functional>

namespace year_2023::day19
{

struct Item
{
	int X;
	int M;
	int A;
	int S;
};

struct Range
{
	long long begin = 1;
	long long end = 4001;
};

struct Ranges
{
	Range X;
	Range M;
	Range A;
	Range S;

	bool isEmpty() const;
	static Ranges createEmpty();
};

struct ICondition
{
	virtual bool match(const Item& item) = 0;
	virtual std::pair<Ranges, Ranges> split(Ranges) = 0;  // (not matched)/(matched) 
};

struct AllwaysTrue : public ICondition
{
	bool match(const Item& item) override { return true; };
	std::pair<Ranges, Ranges> split(Ranges ranges) override { return std::make_pair(Ranges::createEmpty(), ranges); };
};

template <int Item::*T, Range Ranges::*TRange>
struct Condition : public ICondition
{
	Condition(
		std::function<bool(int lhs)> operation,
		std::function<std::pair<Range, Range>(Range)> splitOperation)
		: operation_(operation)
		, splitOperation_(splitOperation)
	{}
	bool match(const Item& item) override
	{
		return operation_( item.*T );
	}

	std::pair<Ranges, Ranges> split(Ranges ranges) override
	{
		auto notMatchedRanges = ranges;
		auto matchedRanges = ranges;
		auto [notMatched, matched] = splitOperation_(ranges.*TRange);
		notMatchedRanges.*TRange = notMatched;
		matchedRanges.*TRange = matched;

		return std::make_pair(notMatchedRanges, matchedRanges);
	}

	std::function<bool(int lhs)> operation_;
	std::function<std::pair<Range, Range>(Range)> splitOperation_;
};

struct PipeElement;

struct Pipe
{
	void exec(const Item& item);
	void exec(Ranges ranges);
	std::vector<std::unique_ptr<PipeElement>> elements;
};

struct PipeElement
{
	bool exec(const Item& item);
	Ranges exec(Ranges item);
	std::unique_ptr<ICondition> condition;
	std::function<void(const Item& item)> operation;
	std::function<void(Ranges eange)> rangeOperation;
};


using InputType = std::vector<Item>;
InputType parse();

class Solution
{
public:
	long long solve(const InputType& input) const;
	long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day19

