#include "Solution.hpp"

#include <cstdlib>
#include <sstream>
#include <set>
#include <parsers/parsers.hpp>
#include <utility/PointRowCol.hpp>

namespace day9
{

const char* fileLoc = "day9/input.txt";

std::vector<Command> parse()
{
    std::vector<Command> out;
    for (auto&& line : parsers::LinesInFileRange(fileLoc))
    {
        Command c;
        std::stringstream ss(line);
        ss >> c.dir >> c.n;
        out.push_back(c);
    }

    return out;
}

namespace
{

PointRowCol getDiff(const char c)
{
    if (c == 'D') return BottomPointDiff;
    if (c == 'U') return UpperPointDiff;
    if (c == 'L') return LeftPointDiff; 
    if (c == 'R') return RightPointDiff;

    throw 1;
}

auto tailShallMove(const PointRowCol& headPosition, PointRowCol& tailPosition)
{
    if (abs(headPosition.row - tailPosition.row) > 1 )
    {
        return true;
    }
    if (abs(headPosition.col - tailPosition.col) > 1 )
    {
        return true;
    }
    return false;
}

auto getTailDir(const PointRowCol& headPosition, const PointRowCol& tailPosition)
{
    return sign(headPosition - tailPosition);
}

template <int TNodes>
auto simulateRope(const std::vector<Command> &in)
{
    std::set<PointRowCol> uniquePositions;

    std::vector<PointRowCol> lineNodes{TNodes, {{},{}}};
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
