#pragma once

#include <vector>
#include <utility>

#include <utility/PointXY.hpp>

namespace day15
{

using SensorLocation = PointXY;
using BeaconLocation = PointXY;

std::vector<std::pair<SensorLocation, BeaconLocation> > parse();

struct Solution
{
    int solve(std::vector<std::pair<SensorLocation, BeaconLocation> > in);
    unsigned long long solve_part2(std::vector<std::pair<SensorLocation, BeaconLocation> > in);
};

}
