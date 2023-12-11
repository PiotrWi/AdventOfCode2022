#include "Solution.hpp"

#include <sstream>
#include <iostream>

#include <parsers/parsers.hpp>

namespace day10
{

std::vector<Command> parse()
{
    std::vector<Command> out;

    for (auto&& line: parsers::getFile(2022, 10))
    {
        Command c;
        std::stringstream ss(line);
        ss >> c.command;
        if (c.command == "addx")
        {
            ss >> c.arg;
        }
        out.push_back(c);
    }

    return out;
}

int Solution::solve(const std::vector<Command>& in)
{
    int x = 1;
    std::string ouy;
    std::vector<int> timeline;

    for (auto&& c: in)
    {
        if (c.command == "noop")
        {
            timeline.push_back(x);
        }
        if (c.command == "addx")
        {
            timeline.push_back(x);
            x += c.arg;
            timeline.push_back(x);
        }
    }

    return 20*timeline[18] + 60 * timeline[58] + 100*timeline[98] + 140*timeline[138] + 180*timeline[178] + 220*timeline[218];
}

namespace
{

char renderPixel(int cycle, int sprite)
{
    cycle = cycle % 40;
    if ((sprite - 1  == cycle) || (sprite == cycle) || (sprite +1 == cycle))
        return '#';
    else
        return '.';
}

}  // namespace

void Solution::solve_part2(const std::vector<Command>& in)
{
    int x = 1;
    std::string out;
    std::vector<int> timeline;
    out += renderPixel(timeline.size(), x);

    for (auto &&c: in)
    {
        if (c.command == "noop")
        {
            timeline.push_back(x);
            out += renderPixel(timeline.size(), x);
        }
        if (c.command == "addx")
        {
            timeline.push_back(x);
            out += renderPixel(timeline.size(), x);
            x += c.arg;
            timeline.push_back(x);
            out += renderPixel(timeline.size(), x);
        }
    }

    std::cout << out.substr(0,40) << std::endl;
    std::cout << out.substr(40,40) << std::endl;
    std::cout << out.substr(80,40) << std::endl;
    std::cout << out.substr(120,40) << std::endl;
    std::cout << out.substr(160,40) << std::endl;
    std::cout << out.substr(200,40) << std::endl;
}

}  // namespace
