#include "Solution.hpp"

#include <algorithm>
#include <optional>
#include <parsers/parsers.hpp>
#include <utility/PointRowCol.hpp>
#include <deque>
#include <map>
#include <unordered_map>
#include <iostream>

namespace day23
{

const char *fileLoc = "year2022/day23/input.txt";

ElfsSurface parse()
{
    return parsers::parseMatrixOfChars(fileLoc);

}

namespace
{

struct Elf
{
    PointRowCol location;
    std::optional <PointRowCol> desiredLocation;
};

struct ConditionsToDir
{
    PointRowCol pointsNeedToBeEmpty[3];
    PointRowCol destination;
};

PointRowCol allPositionsAround[] = {LeftPointDiff, RightPointDiff, UpperLeftPointDiff,
                                    UpperPointDiff, UpperRightPointDiff,BottomLeftPointDiff,
                                    BottomPointDiff, BottomRightPointDiff};

ConditionsToDir northWish = {{UpperLeftPointDiff, UpperPointDiff, UpperRightPointDiff}, UpperPointDiff};
ConditionsToDir southWish = {{BottomLeftPointDiff, BottomPointDiff, BottomRightPointDiff}, BottomPointDiff};
ConditionsToDir westWish = {{LeftPointDiff, BottomLeftPointDiff, UpperLeftPointDiff}, LeftPointDiff};
ConditionsToDir eastWish = {{RightPointDiff, BottomRightPointDiff, UpperRightPointDiff}, RightPointDiff};

using ElfsMap = std::unordered_map<PointRowCol, Elf>;

template <size_t N>
bool allEmpty(PointRowCol elf, PointRowCol (&pointsToCheck)[N], const ElfsMap& elfMap)
{
    return std::all_of(pointsToCheck, pointsToCheck + N, [&elfMap, &elf](auto&& diffPoint)
    {
        auto point = elf + diffPoint;
        return elfMap.count(point) == 0;
    });
}


ElfsMap initElfs(const ElfsSurface& elfMap)
{
    ElfsMap elfs;
    for (int row = 0; row < (int)elfMap.size(); ++row)
    {
        for (int col = 0; col < (int)elfMap[row].size(); ++col)
        {
            if (elfMap[row][col] == '#')
            {
                elfs[PointRowCol{row, col}] = Elf{{row, col}, {}};
            }
        }
    }
    return elfs;
}

long calculateArea(const ElfsMap& elfs)
{
    int minRow = 100000;
    int maxRow = -100000;
    int minCol = 100000;
    int maxCol = -100000;

    for (auto&& elfPair: elfs)
    {
        const auto& elfLoc = elfPair.second.location;
        minRow = std::min(minRow, elfLoc.row);
        minCol = std::min(minCol, elfLoc.col);
        maxRow = std::max(maxRow, elfLoc.row);
        maxCol = std::max(maxCol, elfLoc.col);
    }
    return ((long)maxRow - minRow + 1) * ((long)maxCol - minCol + 1);
}

[[maybe_unused]]
void print(const ElfsMap& elfs)
{
    int minRow = 100000;
    int maxRow = -100000;
    int minCol = 100000;
    int maxCol = -100000;

    for (auto&& elfPair: elfs)
    {
        const auto& elfLoc = elfPair.second.location;
        minRow = std::min(minRow, elfLoc.row);
        minCol = std::min(minCol, elfLoc.col);
        maxRow = std::max(maxRow, elfLoc.row);
        maxCol = std::max(maxCol, elfLoc.col);
    }

    std::vector<std::vector<char>> toPrint(maxRow - minRow + 1, std::vector<char>(maxCol - minCol + 1, '.'));
    for (auto&& elfPair: elfs)
    {
        toPrint[elfPair.second.location.row - minRow][elfPair.second.location.col - minCol] = '#';
    }

    for (int row = 0; row < (int)toPrint.size(); ++row)
    {
        for (int col = 0; col < (int)toPrint[row].size(); ++col)
        {
            std::cout << toPrint[row][col];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int solveImpl(ElfsMap& elfs, int iterationsLimit)
{
    std::deque<ConditionsToDir> directionWishes{northWish, southWish, westWish, eastWish};
    int iterations = 0;
    bool anyMoved = true;

    for (; iterations < iterationsLimit && anyMoved; ++iterations)
    {
        anyMoved = false;
        std::unordered_map<PointRowCol, int> destinationWishes;

        for (auto& elf : elfs)
        {
            if (allEmpty(elf.first, allPositionsAround, elfs))
            {
                continue;
            }
            for (auto&& wish: directionWishes)
            {
                if (allEmpty(elf.first, wish.pointsNeedToBeEmpty, elfs))
                {
                    auto& desiredLoc = elf.second.desiredLocation;
                    desiredLoc = elf.first + wish.destination;
                    ++destinationWishes[*desiredLoc];
                    break;
                }
            }
        }

        for (auto& elfPair : elfs)
        {
            auto& elf = elfPair.second;
            auto& desiredLocation = elf.desiredLocation;
            if (desiredLocation && destinationWishes[*desiredLocation] == 1)
            {
                elf.location = *elf.desiredLocation;
                anyMoved = true;
            }
            elf.desiredLocation = {};
        }

        auto newLocationMap = ElfsMap();
        for (auto&& elf: elfs)
        {
            newLocationMap[elf.second.location] = elf.second;
        }
        std::swap(newLocationMap, elfs);

        directionWishes.push_back(directionWishes.front());
        directionWishes.pop_front();
    }

    return iterations;
}

}  // namespace

int Solution::solve(const ElfsSurface& elfsSurface)
{
    auto elfs = initElfs(elfsSurface);
    solveImpl(elfs, 10);

    return calculateArea(elfs) - elfs.size();
}

int Solution::solve_part2(const ElfsSurface& elfsSurface)
{
    auto elfs = initElfs(elfsSurface);

    return solveImpl(elfs, 100000);
}

}  //namespace day23
