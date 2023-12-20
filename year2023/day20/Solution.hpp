#pragma once

#include <map>
#include <memory>

#include <utility>
#include <string>
#include <vector>

namespace year_2023::day20
{

struct Signal
{
	std::string from;
	std::string to;
	bool signal;
};

struct INode
{
	virtual void init() = 0;
	virtual std::vector<Signal> react(bool signal, std::string from) = 0;
	virtual void registerInput(std::string from) = 0;
	virtual bool getState() const = 0;
	virtual std::vector<std::string> getNeighbours() = 0;
};

using InputType = std::map<std::string, std::unique_ptr<INode> >;
InputType parse();

class Solution
{
public:
	long long solve(InputType& input) const;
	long long solve_part2(InputType& input) const;
};

}  // namespace year_2023::day20
