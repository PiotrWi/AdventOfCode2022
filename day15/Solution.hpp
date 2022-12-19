#pragma once

#include <vector>
#include <utility>

#include <utility/Point.hpp>

namespace day15
{

using SensorLocation = Point;
using BeaconLocation = Point;

std::vector<std::pair<SensorLocation, BeaconLocation> > parse();

struct Solution
{
    int solve(std::vector<std::pair<SensorLocation, BeaconLocation> > in);
    unsigned long long solve_part2(std::vector<std::pair<SensorLocation, BeaconLocation> > in);
};

}
