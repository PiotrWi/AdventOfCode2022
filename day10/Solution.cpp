#include "Solution.hpp"

#include <fstream>
#include <sstream>

namespace day10
{

const char* fileLoc = "day10/input.txt";

std::vector<Command> parse()
{
    std::vector<Command> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    while (inputFile) {
        std::getline(inputFile, line);
        if (line == "") {
            continue;
        }
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

int Solution::solve_part2(const std::vector<Command>&) { return {}; }


}  // namespace
