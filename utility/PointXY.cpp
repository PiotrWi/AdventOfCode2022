#include "PointXY.hpp"

PointXY operator+(PointXY lhs, PointXY rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

PointXY operator-(PointXY lhs, PointXY rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

int manhatanAbs(PointXY lhs)
{
    return abs(lhs.x) + abs(lhs.y);
}

