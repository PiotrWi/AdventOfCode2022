#pragma once

#include <vector>

namespace day14
{

struct SingleScanEntry
{
    int collumn = 0;
    int depth = 0;
};

using SingleScanPath = std::vector<SingleScanEntry>;

std::vector<SingleScanPath> parse();

struct Solution
{
    int solve(std::vector<SingleScanPath>);
    int solve_part2(std::vector<SingleScanPath> in);

};

}
