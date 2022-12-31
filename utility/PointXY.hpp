#pragma once

#include <ostream>

struct PointXY
{
    int x;
    int y;
};

PointXY operator+(PointXY lhs, PointXY rhs);
PointXY operator-(PointXY lhs, PointXY rhs);
int manhatanAbs(PointXY lhs);
