#include "Solution.hpp"

#include <fstream>
#include <sstream>
#include <set>
#include <utility>
#include <iostream>

namespace day9
{

const char* fileLoc = "day9/input.txt";

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
        ss >> c.dir >> c.n;
        out.push_back(c);
    }

    return out;
}

namespace
{

std::pair<int, int> getDiff(const char c)
{
    if (c == 'D')
    {
        return {-1, 0};
    }
    if (c == 'U')
    {
        return {1, 0};
    }
    if (c == 'L')
    {
        return {0, -1};
    }
    if (c == 'R')
    {
        return {0, 1};
    }
    throw 1;
}

std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    return {lhs.first + rhs.first, lhs.second+ rhs.second};
}

auto tailShallMove(const std::pair<int, int>& headPosition, const std::pair<int, int>& tailPosition)
{
    if (abs(headPosition.first - tailPosition.first) > 1 )
    {
        return true;
    }
    if (abs(headPosition.second - tailPosition.second) > 1 )
    {
        return true;
    }
    return false;
}

auto sign(int arg)
{
    if (arg > 0)
    {
        return 1;
    }
    if (arg < 0)
    {
        return -1;
    }
    return 0;
}

auto getTailDir(const std::pair<int, int>& headPosition, const std::pair<int, int>& tailPosition)
{
    return std::make_pair(sign(headPosition.first-tailPosition.first), sign(headPosition.second - tailPosition.second));
}

template <int TNodes>
auto simulateRope(const std::vector<Command> &in)
{
    std::set<std::pair<int, int>> uniquePositions;

    std::vector<std::pair<int, int>> lineNodes{TNodes, {{},{}}};
    auto& headPosition = lineNodes[0];
    auto& tailPosition =  lineNodes[TNodes-1];
    uniquePositions.insert(tailPosition);

    for (auto&& c: in)
    {
        for (int _ = 0; _ < c.n; ++_)
        {
            headPosition = headPosition + getDiff(c.dir);

            for (int i = 1; i < TNodes; ++i)
            {
                if (tailShallMove(lineNodes[i-1], lineNodes[i]))
                {
                    lineNodes[i] = lineNodes[i] + getTailDir(lineNodes[i-1], lineNodes[i]);
                }
            }
            uniquePositions.insert(tailPosition);
        }
    }

    return uniquePositions.size();
}

}  // namespace

int Solution::solve(const std::vector<Command> &in)
{
    return simulateRope<2>(in);
}
int Solution::solve_part2(const std::vector<Command>& in)
{
    return simulateRope<10>(in);
}

}  // namespace day9
