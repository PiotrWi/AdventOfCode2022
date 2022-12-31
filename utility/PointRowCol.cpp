#include "PointRowCol.hpp"

namespace
{
int abs_i(int arg)
{
    if (arg <0)
        return -1*arg;
    return arg;
}

}

PointRowCol operator+(const PointRowCol& lhs, const PointRowCol& rhs)
{
    return {lhs.row + rhs.row, lhs.col + rhs.col};
}

PointRowCol operator-(const PointRowCol& lhs, const PointRowCol& rhs)
{
    return {lhs.row - rhs.row, lhs.col - rhs.col};
}

bool operator==(const PointRowCol& lhs, const PointRowCol &rhs)
{
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

bool operator<(const PointRowCol& lhs, const PointRowCol& rhs)
{
    if (lhs.row < rhs.row)
    {
        return true;
    }
    if (lhs.row > rhs.row)
    {
        return false;
    }
    return lhs.col < rhs.col;
}

PointRowCol to1BaseIndexing(PointRowCol point)
{
    return {point.row + 1, point.col + 1};
}

std::ostream& operator << (std::ostream& str, const PointRowCol& point)
{
    str << "(row = " << point.row << ", col = " << point.col << ")";
    return str;
}

int manhatanAbs(PointRowCol lhs)
{
    return abs_i(lhs.row) + abs_i(lhs.col);
}
