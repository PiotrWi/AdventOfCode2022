#include "Solution.hpp"

#include <ranges>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day19
{

bool Ranges::isEmpty() const
{
	return X.begin >= X.end && A.begin >= A.end && M.begin >= M.end && S.begin >= S.end;
}

Ranges Ranges::createEmpty()
{
	return { {0, 0}, {0, 0}, {0, 0}, {0, 0}	};
}

bool PipeElement::exec(const Item& item)
{
	if (condition->match(item))
	{
		operation(item);
		return true;
	}
	return false;
}

Ranges PipeElement::exec(Ranges ranges)
{
	auto [notMatched, matched] = condition->split(ranges);
	rangeOperation(matched);
	return notMatched;
}

void Pipe::exec(const Item& item)
{
	for (auto&& pipeElem : elements)
	{
		if (pipeElem->exec(item))
		{
			return;
		}
	}
	throw 1;
}

void Pipe::exec(Ranges ranges)
{
	auto notProcedPart = ranges;
	for (auto&& pipeElem : elements)
	{
		notProcedPart = pipeElem->exec(notProcedPart);
	}
}

namespace
{

std::vector<Item> accepted;
std::vector<Ranges> acceptedRanges;

std::map <std::string, Pipe> pipes;

std::unique_ptr<ICondition> createConcreteCondition(char c,
	std::function<bool(int lhs)> fcn,
	std::function<std::pair<Range, Range>(Range)> splitOperation)
{
	if (c == 'x') return std::make_unique<Condition<&Item::X, &Ranges::X>>( fcn, splitOperation);
	if (c == 'm') return std::make_unique<Condition<&Item::M, &Ranges::M>>( fcn, splitOperation);
	if (c == 'a') return std::make_unique<Condition<&Item::A, &Ranges::A>>( fcn, splitOperation);
	return std::make_unique<Condition<&Item::S, &Ranges::S>>(fcn, splitOperation);
}

std::unique_ptr<ICondition> parseCondition(std::string conditionStr)
{
	auto lessFound = conditionStr.find('<') != std::string::npos;
	auto itemFeatureToValue = splitAndTrim(conditionStr, lessFound ? '<' : '>');
	auto featureChar = itemFeatureToValue[0][0];
	auto rhs = std::stoi(itemFeatureToValue[1]);

	if (lessFound)
	{
		return createConcreteCondition(featureChar,
			[rhs](int arg) {return arg < rhs; },
			[rhs](Range arg) { return std::make_pair(Range{ rhs, arg.end }, Range{arg.begin, rhs}); }
		);
	}
	return createConcreteCondition(featureChar,
		[rhs](int arg) {return arg > rhs; },
		[rhs](Range arg) { return std::make_pair(Range{ arg.begin, rhs + 1 }, Range{ rhs + 1, arg.end }); }
	);
}

void parsePipe(std::string line)
{
	Pipe pipe;

	auto labelToTail = splitAndTrim(line, '{');
	std::string label = labelToTail[0];
	auto tail = labelToTail[1].substr(0, labelToTail[1].size() - 1);

	for (auto&& singleInstruction : splitAndTrim(tail, ','))
	{
		auto conditionToAction = splitAndTrim(singleInstruction, ':');
		std::unique_ptr<ICondition> condition;
		std::string operationString;
		if (conditionToAction.size() == 1)
		{
			condition = std::make_unique<AllwaysTrue>();
			operationString = conditionToAction[0];
		}
		else
		{
			condition = parseCondition(conditionToAction[0]);
			operationString = conditionToAction[1];
		}

		std::unique_ptr<PipeElement> pipeElement;
		if (operationString == "A")
		{
			pipeElement = std::make_unique<PipeElement>(std::move(condition),
				[](const Item& item) { accepted.push_back(item); },
				[](Ranges ranges) { acceptedRanges.push_back(ranges); }
			);
		}
		else if (operationString == "R")
		{
			pipeElement = std::make_unique<PipeElement>(std::move(condition),
				[](const Item&) {},
				[](Ranges) {}
			);
		}
		else
		{
			pipeElement = std::make_unique<PipeElement>(std::move(condition),
				[label = operationString](const Item& item) { pipes[label].exec(item); },
				[label = operationString](Ranges ranges) { pipes[label].exec(ranges); }
			);
		}
		pipe.elements.emplace_back(std::move(pipeElement));
	}
	pipes[label] = std::move(pipe);
}

}  // namespace

InputType parse()
{
	auto file = parsers::getFile(2023, 19);

	auto it = file.begin();
	for (; not (*it).empty(); ++it)
	{
		parsePipe(*it);
	}
	++it;

	std::vector<Item> items;
	for (; it != file.end(); ++it)
	{
        auto numbersAndChars = splitNumbersAndChars(*it);
		auto numbers = numbersAndChars
			| std::views::filter([](auto&& s) { return std::isdigit(s[0]); })
			| std::views::transform([](auto&& s) { return std::stoi(s); })
			| To<std::vector<int>>{};
		items.push_back(Item{ numbers [0], numbers[1], numbers[2], numbers[3]});
	}
	return items;
}

long long Solution::solve(const InputType& input) const
{
	for (auto&& elem : input)
	{
		pipes["in"].exec(elem);
	}

	auto sum = 0ll;
	for (auto&& elem : accepted)
	{
		sum += elem.X + elem.A + elem.M + elem.S;
	}
	return sum;
}

long long Solution::solve_part2(const InputType&) const
{
	Ranges wholeArea;
	pipes["in"].exec(wholeArea);

	auto sum = 0ll;
	for (auto&& elem : acceptedRanges)
	{
		sum += (elem.A.end - elem.A.begin) * (elem.X.end - elem.X.begin) * (elem.M.end - elem.M.begin) * (elem.S.end - elem.S.begin);
	}

	return sum; 
}
	
}  // namespace year_2023::day19
