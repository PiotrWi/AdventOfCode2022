#include "PointXY.hpp"

namespace internal
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
    return internal::abs(lhs.x) + internal::abs(lhs.y);
}

