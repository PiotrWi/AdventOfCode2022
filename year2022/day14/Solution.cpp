#include "Solution.hpp"
#include <StringAlgorithms/StringAlgorithms.hpp>

#include <parsers/parsers.hpp>

namespace day14
{

const char* fileLoc = "year2022/day14/input.txt";

std::vector<SingleScanPath> parse()
{
    std::vector<SingleScanPath> out;

    for (auto&& line: parsers::LinesInFileRange(fileLoc))
    {
        SingleScanPath singleReadout;
        for (auto&& singleRead: splitAndTrimByStr(line, "->"))
        {
            auto singleScanStr = splitAndTrim(singleRead, ',');
            SingleScanEntry sse;
            sse.collumn = std::stoi(singleScanStr[0]);
            sse.depth = std::stoi(singleScanStr[1]);
            singleReadout.push_back(sse);
        }
        out.push_back(singleReadout);
    }

    return out;
}

namespace
{

SingleScanEntry operator-(SingleScanEntry lhs, SingleScanEntry rhs)
{
    SingleScanEntry out;
    out.depth = lhs.depth - rhs.depth;
    out.collumn = lhs.collumn - rhs.collumn;
    return out;
}

SingleScanEntry operator+(SingleScanEntry lhs, SingleScanEntry rhs)
{
    SingleScanEntry out;
    out.depth = lhs.depth + rhs.depth;
    out.collumn = lhs.collumn + rhs.collumn;
    return out;
}

int sign(int i)
{
    if (i>0) return 1;
    if (i<0) return -1;
    return 0;
}

SingleScanEntry sign(SingleScanEntry in)
{
    SingleScanEntry out;
    out.collumn = sign(in.collumn);
    out.depth = sign(in.depth);
    return out;
}

bool operator==(SingleScanEntry lhs, SingleScanEntry rhs)
{
    return lhs.depth == rhs.depth && lhs.collumn == rhs.collumn;
}

void drawLine(std::vector<std::vector <char> >& occupancy, SingleScanEntry source, SingleScanEntry destination)
{
    auto diff = sign(destination - source);
    for (SingleScanEntry it = source; it != destination; it = it + diff)
    {
        occupancy[it.depth][it.collumn] = '#';
    }
    occupancy[destination.depth][destination.collumn] = '#';
}

SingleScanEntry toDownDiff = {0, 1};
SingleScanEntry toBottomLeftDiff = {-1, 1};
SingleScanEntry toBottomRightDiff = {1, 1};


bool isDownAvailable(std::vector<std::vector <char> >& occupancy, SingleScanEntry point)
{
    auto dest = point + toDownDiff;
    return occupancy[dest.depth][dest.collumn] == '.';
}

bool isBottomLeftAvailable(std::vector<std::vector <char> >& occupancy, SingleScanEntry point)
{
    auto dest = point + toBottomLeftDiff;
    return occupancy[dest.depth][dest.collumn] == '.';
}

bool isBottomRightAvailable(std::vector<std::vector <char> >& occupancy, SingleScanEntry point)
{
    auto dest = point + toBottomRightDiff;
    return occupancy[dest.depth][dest.collumn] == '.';
}

bool simulateSandFalling(std::vector<std::vector <char> >& occupancy)
{
    SingleScanEntry point = {500, 0};
    while (true)
    {
        while (isDownAvailable(occupancy, point))
        {
            point = point + toDownDiff;
            if (point.depth == (int)occupancy.size()-1) return false;
        }
        if (isBottomLeftAvailable(occupancy, point))
        {
            point = point + toBottomLeftDiff;
            if (point.depth == (int)occupancy.size()-1) return false;
            if (point.collumn == 0) return false;
            continue;
        }
        if (isBottomRightAvailable(occupancy, point))
        {
            point = point + toBottomRightDiff;
            if (point.depth == (int)occupancy.size()-1) return false;
            if (point.collumn == (int)occupancy[0].size()-1) return false;
            continue;
        }
        if (isDownAvailable(occupancy, point))
        {
            continue;
        }
        occupancy[point.depth][point.collumn] = 'o';
        break;
    }
    return true;
}

auto getSize(const std::vector<SingleScanPath>& in)
{
    int maxCollumn = 0;
    int maxDepth = 0;
    for (auto&& singlePath: in)
    {
        for (auto&& singleScan: singlePath)
        {
            if (singleScan.depth > maxDepth)
            {
                maxDepth = singleScan.depth;
            }
            if (singleScan.collumn > maxCollumn)
            {
                maxCollumn = singleScan.collumn;
            }
        }
    }
    return std::make_pair(maxCollumn, maxDepth);
}

void drawScanReadouts(const std::vector<SingleScanPath>& in, std::vector<std::vector <char> >& occupancy)
{
    for (auto&& singlePath: in)
    {
        for (auto i = 0u; i < singlePath.size() - 1; ++i)
        {
            auto& source = singlePath[i];
            auto& destination = singlePath[i+1];
            drawLine(occupancy, source, destination);
        }
    }
}

void drawFloor(int maxDepth, std::vector<std::vector <char> >& occupancy)
{
    for (auto col = 0u; col < occupancy[0].size(); ++col)
    {
        occupancy[maxDepth+2][col] = '#';
    }
}

}  // namespace

int Solution::solve(std::vector<SingleScanPath> in)
{
    const auto&[maxColumn, maxDepth]  = getSize(in);
    std::vector<std::vector <char> > occupancy(maxDepth+2, std::vector(maxColumn + 2, '.'));
    drawScanReadouts(in, occupancy);

    int counter = 0;
    while (simulateSandFalling(occupancy)) counter++;

    return counter;
}

int Solution::solve_part2(std::vector<SingleScanPath> in)
{
    const auto&[maxColumn, maxDepth]  = getSize(in);
    std::vector<std::vector <char> > occupancy(maxDepth+3, std::vector(1500, '.'));

    drawScanReadouts(in, occupancy);
    drawFloor(maxDepth, occupancy);

    int counter = 0;
    while (true)
    {
        simulateSandFalling(occupancy);
        counter++;
        if (occupancy[0][500] == 'o')
        {
            break;
        }
    }

    return counter;
}

}
