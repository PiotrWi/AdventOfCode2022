#include "Solution.hpp"

#include <algorithm>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace day11
{

const char* fileLoc = "day11/input.txt";

namespace
{

std::function<unsigned long long(unsigned long long)> parseOldOrVal(const std::string& in)
{
    if (in == "old")
    {
        return [](auto in){ return in; };
    }
    return [val = std::stoi(in)](auto&&){ return val; };
}

Equasion parseEquasion(const std::string& in)
{
    Equasion out;

    auto rightSideStr = splitAndTrim(in, '=')[1];
    char oper;
    std::string lhs;
    std::string rhs;

    if (rightSideStr.find('+') != std::string::npos)
    {
        out.operation = []( auto&& lhs, auto&& rhs) {return lhs+rhs; };
        oper = '+';
    }
    else
    {
        out.operation = []( auto&& lhs, auto&& rhs) {return lhs*rhs; };
        oper = '*';
    }
    auto tmp = splitAndTrim(rightSideStr, oper);
    lhs = tmp[0];
    rhs = tmp[1];

    out.lhsGetter = parseOldOrVal(lhs);
    out.rhsGetter = parseOldOrVal(rhs);

    return out;
}

}

std::vector<Monkey> parse()
{
    std::vector<Monkey> out;

    for (auto&& line: parsers::LinesInFileRange(fileLoc))
    {
        std::string item;

        line = ltrim(line);
        auto separatorPos = line.find(':');
        item = line.substr(0, separatorPos);
        if (item.starts_with("Monkey"))
        {
            Monkey m;
            m.monkeyId = out.size();
            out.push_back(m);
            continue;
        }

        auto payload = trim(line.substr(separatorPos + 1));
        Monkey& m = out.back();
        if (item == "Starting items")
        {
            for (auto&& itemStr: splitAndTrim(payload, ','))
            {
                m.startingItems.emplace_back(std::stoi(itemStr));
            }
        }
        if (item == "Operation")
        {
            m.operation = parseEquasion(payload);
        }

        if (item == "Test")
        {
            m.testModulo = stoi(splitAndTrim(payload, ' ')[2]);
        }

        if (item == "If true")
        {
            m.monkeyGiven[true] = stoi(splitAndTrim(payload, ' ')[3]);
        }

        if (item == "If false")
        {
            m.monkeyGiven[false] = stoi(splitAndTrim(payload, ' ')[3]);
        }
    }

    return out;
}

unsigned long long Solution::solve(std::vector<Monkey> in)
{
    unsigned long long moduler = 1;
    for (auto&& monkey: in)
    {
        moduler *= monkey.testModulo;
    }

    for (auto round = 0; round < 20; ++round)
    {
        for (auto&& monkey: in)
        {
            std::vector<unsigned long long> itemsBeingInspected = {};
            std::swap(itemsBeingInspected, monkey.startingItems);
            monkey.inspectedItems += itemsBeingInspected.size();
            for (auto item: itemsBeingInspected)
            {
                auto newWorryLevel = monkey.operation(item) / 3;
                newWorryLevel = newWorryLevel % moduler;
                auto test = (newWorryLevel % monkey.testModulo) == 0;
                auto monkeyReceiver = monkey.monkeyGiven[test];
                in[monkeyReceiver].startingItems.push_back(newWorryLevel);
            }
        }
    }

    std::sort(in.begin(), in.end(), [](auto&& lhs, auto&& rhs) { return lhs.inspectedItems > rhs.inspectedItems; } );
    return in[0].inspectedItems * in[1].inspectedItems;
}

unsigned long long Solution::solve_part2(std::vector<Monkey> in)
{
    unsigned long long moduler = 1;
    for (auto&& monkey: in)
    {
        moduler *= monkey.testModulo;
    }

    for (auto round = 0; round < 10000; ++round)
    {
        for (auto&& monkey: in)
        {
            std::vector<unsigned long long> itemsBeingInspected = {};
            std::swap(itemsBeingInspected, monkey.startingItems);
            monkey.inspectedItems += itemsBeingInspected.size();
            for (auto item: itemsBeingInspected)
            {
                auto newWorryLevel = monkey.operation(item);
                newWorryLevel = newWorryLevel % moduler;
                auto test = (newWorryLevel % monkey.testModulo) == 0;
                auto monkeyReceiver = monkey.monkeyGiven[test];
                in[monkeyReceiver].startingItems.push_back(newWorryLevel);
            }
        }
    }

    std::sort(in.begin(), in.end(), [](auto&& lhs, auto&& rhs) { return lhs.inspectedItems > rhs.inspectedItems; } );
    return in[0].inspectedItems * in[1].inspectedItems;
}

}  // namespace day11
