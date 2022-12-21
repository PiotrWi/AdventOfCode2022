#pragma once

#include <vector>

namespace day18
{

struct Point3d
{
    int x;
    int y;
    int z;
};

std::vector<Point3d> parse();

struct Solution
{
    int solve(std::vector<Point3d> points);
    int solve_part2(std::vector<Point3d> points);

};

}  // namespace day18
