#include "Solution.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility/PointXY.hpp>

namespace day17
{

const char *fileLoc = "day17/input.txt";

std::vector<WindDirrection> parse()
{
    std::vector<WindDirrection> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    std::getline(inputFile, line);
    std::stringstream ss(line);
    char c;
    while (ss.get(c)) {
        out.push_back(c == '<' ? WindDirrection::left : WindDirrection::right);
    }

    inputFile.close();
    return out;
}

namespace
{

long getTallestPoint (std::vector<std::string>& tunel)
{
    auto it = std::find_if(tunel.rbegin(), tunel.rend(), [](auto&& line)
    {
        return line != "|       |";
    });
    return tunel.size() - (it-tunel.rbegin()) - 1;
}

auto addEmptyLinesForFigure(std::vector<std::string>& tunel, int tallestPoint, int figureSize)
{
    auto lackOfLines = tallestPoint + 4 + figureSize - (int)tunel.size();
    for (auto i = 0; i < lackOfLines; ++i)
    {
        tunel.push_back("|       |");
    }
}

std::vector<PointXY> addFigure(std::vector<std::string>& tunel, int tallestPoint, long i)
{
    const int horizontalLine = 0;
    const int plus = 1;
    const int LShape = 2;
    const int verticalLine = 3;
    const int square = 4;

    if (i % 5 == horizontalLine)
    {
        addEmptyLinesForFigure(tunel, tallestPoint, 1);
        return {{3, tallestPoint + 4}, {4, tallestPoint + 4}, {5, tallestPoint + 4}, {6, tallestPoint + 4}};
    }
    if (i % 5 == plus)
    {
        addEmptyLinesForFigure(tunel, tallestPoint, 3);
        return {{4, tallestPoint + 6}, {3, tallestPoint + 5}, {4, tallestPoint + 5}, {5, tallestPoint + 5}, {4, tallestPoint+4}};
    }
    if (i % 5 == LShape)
    {
        addEmptyLinesForFigure(tunel, tallestPoint, 3);
        return {{5, tallestPoint + 6}, {5, tallestPoint + 5}, {3, tallestPoint + 4}, {4, tallestPoint + 4}, {5, tallestPoint+4}};
    }
    if (i % 5 == verticalLine)
    {
        addEmptyLinesForFigure(tunel, tallestPoint, 4);
        return {{3, tallestPoint + 7}, {3, tallestPoint + 6}, {3, tallestPoint + 5}, {3, tallestPoint + 4}};
    }
    if (i % 5 == square)
    {
        addEmptyLinesForFigure(tunel, tallestPoint, 2);
        return {{3, tallestPoint + 5}, {4, tallestPoint + 5}, {3, tallestPoint + 4}, {4, tallestPoint + 4}};
    }
    return {};
}

[[maybe_unused]]
void printTunel(const std::vector<std::string>& tunel)
{
    std::cout << std::endl;

    for (auto it = tunel.rbegin(); it != tunel.rend(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;
}

auto tryToMoveWithWind(const std::vector<std::string>& tunel, std::vector<PointXY>& cords, WindDirrection w)
{
    PointXY diff;
    if (w == WindDirrection::left)
    {
        diff = PointXY{-1, 0};
    }
    else
    {
        diff = PointXY{1, 0};
    }

    for (auto it = cords.begin(); it != cords.end(); ++it)
    {
        *it = *it + diff;
        if (tunel[it->y][it->x] != ' ')
        {
            for (auto revertIt = cords.begin(); revertIt <= it; ++revertIt)
            {
                *revertIt = *revertIt - diff;
            }
            return;
        }
    }
}
auto tryToMoveDown(const std::vector<std::string>& tunel, std::vector<PointXY>& cords)
{
    PointXY diff;
    diff = PointXY{0, -1};

    for (auto it = cords.begin(); it != cords.end(); ++it)
    {
        *it = *it + diff;
        if (tunel[it->y][it->x] != ' ')
        {
            for (auto revertIt = cords.begin(); revertIt <= it; ++revertIt)
            {
                *revertIt = *revertIt - diff;
            }
            return false;
        }
    }
    return true;
}
auto writeFigureToTunel(std::vector<std::string>& tunel, const std::vector<PointXY>& cords)
{
    for (auto&& c : cords)
    {
        tunel[c.y][c.x] = '#';
    }
}

}  // namespace



int Solution::solve(std::vector<WindDirrection> in)
{
    std::vector<std::string> tunel;
    tunel.push_back({"+-------+"});

    int windPointer = 0;

    for (unsigned long i = 0; i < 2022; ++i)
    {
        auto tallestPoint = getTallestPoint(tunel);
        auto cords = addFigure(tunel, tallestPoint, i);

        while (true)
        {
            tryToMoveWithWind(tunel, cords, in[windPointer]);
            ++windPointer;
            windPointer = windPointer % in.size();
            if (not tryToMoveDown(tunel, cords))
            {
                break;
            }
        }

        writeFigureToTunel(tunel, cords);

    }
    auto tallestPoint = getTallestPoint(tunel);
    return tallestPoint;
}

namespace
{

struct Memorial
{
    int rockIndex;
    int tallestPoint;
    std::vector<std::string> tunnelTop;
};

std::vector<std::string> getTunnelTop(const std::vector<std::string>& tunel)
{
    std::string matchMap = "|       |";
    std::vector<std::string> out;

    auto it = tunel.rbegin();
    for (; it != tunel.rend(); ++it)
    {
        for (unsigned int i = 0; i < matchMap.size(); ++i)
        {
            if ((*it)[i] != ' ')
            {
                matchMap[i] = '#';
            }
        }
        out.push_back(*it);
        if (matchMap == "#########")
        {
            break;
        }
    }

    std::reverse(out.begin(), out.end());
    return out;
}

const long secondTaskDone = 1000000000000;
}

long long Solution::solve_part2(std::vector<WindDirrection> in)
{
    std::vector<std::string> tunel;
    tunel.push_back({"+-------+"});

    int windPointer = 0;

    std::vector<Memorial> cycles;

    for (unsigned long i = 0; i < secondTaskDone; ++i)
    {
        // This cycle finding is in fact done based on the observation.
        // Different set of datas may have different cycles, so it is not robouts.
        // Correct algorithms shall do very similar job, but it shall find for a pattern in different situations with
        // same preconditions for points in cycle:
        // EG:
        //     wind cycle need to be same
        //     shape falling shall be same
        // And after then we can do a check. Whether it is a cycle.
        if (windPointer == 0 && i != 0)
        {
            auto top = getTunnelTop(tunel);
            cycles.emplace_back(i, getTallestPoint(tunel), top);
            if (cycles.size() == 3)
                break;
        }

        auto tallestPoint = getTallestPoint(tunel);
        auto cords = addFigure(tunel, tallestPoint, i);

        while (true)
        {
            tryToMoveWithWind(tunel, cords, in[windPointer]);
            ++windPointer;
            windPointer = windPointer % in.size();
            if (not tryToMoveDown(tunel, cords))
            {
                break;
            }
        }

        writeFigureToTunel(tunel, cords);
    }

    int firstCycleEnd = cycles[0].rockIndex;
    auto secondCycleEnd = cycles[1].rockIndex;
    auto cyclesDiff = secondCycleEnd - firstCycleEnd;

    auto notCatchToNextCycle = 0ull;

    unsigned long tallestPointSoFar = cycles[0].tallestPoint;
    for (unsigned long i = firstCycleEnd; i < secondTaskDone; i+=cyclesDiff)
    {
        tallestPointSoFar += cycles[1].tallestPoint - cycles[0].tallestPoint;
        notCatchToNextCycle = i;
    }
    tallestPointSoFar -= cycles[1].tallestPoint - cycles[0].tallestPoint;

    tunel = cycles[1].tunnelTop;
    long topOfTheCycleEnd = getTallestPoint(tunel);
    windPointer = 0;
    for (unsigned long i = notCatchToNextCycle; i < secondTaskDone; ++i)
    {
        auto tallestPoint = getTallestPoint(tunel);
        auto cords = addFigure(tunel, tallestPoint, i);

        while (true)
        {
            tryToMoveWithWind(tunel, cords, in[windPointer]);
            ++windPointer;
            windPointer = windPointer % in.size();
            if (not tryToMoveDown(tunel, cords))
            {
                break;
            }
        }

        writeFigureToTunel(tunel, cords);
    }
    long topOfTheTail = getTallestPoint(tunel);
    return tallestPointSoFar + topOfTheTail - topOfTheCycleEnd;
}

}   // namespace day17
