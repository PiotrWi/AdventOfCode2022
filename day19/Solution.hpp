#pragma once

#include <vector>

namespace day19
{

struct Resources
{
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;
};

struct Blueprint
{
    Resources oreRobotCost = {};
    Resources clayRobotCost = {};
    Resources obsidianRobotCost = {};
    Resources geodeRobotCost = {};
};

std::vector<Blueprint> parse();

struct Solution
{
public:
    int solve(std::vector<Blueprint>);
    int solve_part2(std::vector<Blueprint>);
};

}