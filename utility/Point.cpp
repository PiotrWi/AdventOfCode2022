#include "Point.hpp"

namespace
{
int abs(int arg)
{
    if (arg <0)
        return -1*arg;
    return arg;
}

}

Point operator+(Point lhs, Point rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

Point operator-(Point lhs, Point rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

int manhatanAbs(Point lhs)
{
    return abs(lhs.x) + abs(lhs.y);
}