#pragma once

#include <functional>
#include <vector>

namespace day11
{

struct Equasion
{
public:
    unsigned long long operator()(unsigned long long oldVal) const
    {
        return operation(lhsGetter(oldVal), rhsGetter(oldVal));
    }
    std::function <unsigned long long(unsigned long long)> lhsGetter;
    std::function <unsigned long long(unsigned long long)> rhsGetter;
    std::function <unsigned long long(unsigned long long, unsigned long long)> operation;
};

struct Monkey
{
    int monkeyId;
    std::vector<unsigned long long> startingItems;
    Equasion operation;
    int testModulo = 0;
    int monkeyGiven[2] = {};

    unsigned long long inspectedItems = 0;
};

std::vector<Monkey> parse();

struct Solution
{
    unsigned long long solve(std::vector<Monkey>);
    unsigned long long solve_part2(std::vector<Monkey>);
};

}
