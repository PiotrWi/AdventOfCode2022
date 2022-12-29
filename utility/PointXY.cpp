#include "PointXY.hpp"

namespace
{
int abs(int arg)
{
    if (arg <0)
        return -1*arg;
    return arg;
}

}

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

PointRowCol operator+(PointRowCol lhs, PointRowCol rhs)
{
    return {lhs.row + rhs.row, lhs.col + rhs.col};
}