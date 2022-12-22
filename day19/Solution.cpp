#include "Solution.hpp"
#include <fstream>
#include <iostream>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <array>

namespace day19
{

const char *fileLoc = "day19/input.txt";

std::vector<Blueprint> parse()
{
    std::vector<Blueprint> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    while (inputFile)
    {
        std::getline(inputFile, line);
        if (line == "")
        {
            continue;
        }
        auto splited = splitAndTrim(line, ' ');
        Blueprint bp;
        bp.oreRobotCost.ore = std::stoi(splited[6]);
        bp.clayRobotCost.ore = std::stoi(splited[12]);
        bp.obsidianRobotCost.ore = std::stoi(splited[18]);
        bp.obsidianRobotCost.clay = std::stoi(splited[21]);
        bp.geodeRobotCost.ore = std::stoi(splited[27]);
        bp.geodeRobotCost.obsidian = std::stoi(splited[30]);
        out.push_back(bp);
    }

    return out;
}

namespace
{

bool operator>=(const Resources &lhs, const Resources &rhs)
{
    return lhs.ore >= rhs.ore
        && lhs.clay >= rhs.clay
        && lhs.obsidian >= rhs.obsidian
        && lhs.geode >= rhs.geode;
}

Resources operator+(const Resources &lhs, const Resources &rhs)
{
    return {lhs.ore + rhs.ore, lhs.clay + rhs.clay, lhs.obsidian + rhs.obsidian, lhs.geode + rhs.geode};
}

Resources operator-(const Resources &lhs, const Resources &rhs)
{
    return {lhs.ore - rhs.ore, lhs.clay - rhs.clay, lhs.obsidian - rhs.obsidian, lhs.geode - rhs.geode};
}

const Resources oneGeode{0, 0, 0, 1};
const Resources oneObsidian{0, 0, 1, 0};
const Resources oneClay{0, 1, 0, 0};
const Resources oneOre{1, 0, 0, 0};

int depth = 24;

struct Flags
{
    bool notProducedGeode = false;
    bool notProducedObsidian = false;
    bool notProducedClay = false;
    bool notProducedOre = false;
};

Flags emptyFlags = {};

int currentMax = 0;

std::array<int, 100> initHeuristicValue()
{
    std::array<int, 100> out = {};
    for (int i = 1; i < 80; ++i)
    {
        out [i+1] = i + out[i];
    }
    return out;
}

int heuristic(int timeLeft)
{
    static std::array<int, 100> heurisitValue = initHeuristicValue();
    return heurisitValue[timeLeft];
}

int lookFor(const Blueprint& bp, Resources production, Resources produced, int time, Flags flags)
{
    int currentGeodeEstimation = produced.geode + (depth-time)*production.geode;
    int maxGeode = produced.geode + (depth-time)*production.geode;
    if (time >= depth-1)
    {
        return maxGeode;
    }

    if (currentGeodeEstimation + heuristic(depth-time) <= currentMax)
    {
        return currentGeodeEstimation;
    }

    if (produced >= bp.geodeRobotCost && not flags.notProducedGeode)
    {
        maxGeode = std::max(maxGeode, lookFor(bp, production+oneGeode, produced + production - bp.geodeRobotCost, time + 1, emptyFlags));
        flags.notProducedGeode = true;
    }

    if (currentGeodeEstimation + heuristic(depth- (time + 1) ) <= currentMax)
    {
        currentMax = std::max(maxGeode, currentMax);
        return maxGeode;
    }

    if (produced >= bp.obsidianRobotCost && not flags.notProducedObsidian)
    {
        maxGeode = std::max(maxGeode, lookFor(bp, production+oneObsidian, produced + production - bp.obsidianRobotCost, time + 1, emptyFlags));
        flags.notProducedObsidian = true;
    }
    if (produced >= bp.clayRobotCost && not flags.notProducedClay)
    {
        maxGeode = std::max(maxGeode, lookFor(bp, production+oneClay, produced + production - bp.clayRobotCost, time + 1, emptyFlags));
        flags.notProducedClay = true;
    }
    if (produced >= bp.oreRobotCost && not flags.notProducedOre)
    {
        maxGeode = std::max(maxGeode, lookFor(bp, production+oneOre, produced + production - bp.oreRobotCost, time + 1, emptyFlags));
        flags.notProducedOre = true;
    }

    if (flags.notProducedGeode && flags.notProducedObsidian && flags.notProducedClay && flags.notProducedOre)
    {
        currentMax = std::max(currentMax, maxGeode);
        return maxGeode;
    }
    maxGeode = std::max(maxGeode, lookFor(bp, production, produced + production, time + 1, flags));
    currentMax = std::max(currentMax, maxGeode);

    return maxGeode;
}

int calculateBp(const Blueprint& bp)
{
    currentMax = 0;
    auto production = Resources{1, 0, 0, 0};
    auto produced = Resources{0, 0, 0, 0};
    auto time = 0;

    return lookFor(bp, production, produced, time, emptyFlags);
}

}  // namespace


int Solution::solve(std::vector<Blueprint> blueprints)
{
    depth = 24;

    int i = 0;
    int allQualities = 0;
    for (auto&& bp : blueprints)
    {

        allQualities += ++i * calculateBp(bp);
    }
    return allQualities;
}

int Solution::solve_part2(std::vector<Blueprint>blueprints)
{
    depth = 32;

    int allQualities = 1;
    for (int i = 0; i < 3; ++i)
    {
        allQualities *= calculateBp(blueprints[i]);
    }
    return allQualities;
}


}