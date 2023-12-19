#include "PointRowCol.hpp"

#include <cstdlib>

namespace
{

auto sign(int arg)
{
    if (arg > 0) return 1;
    if (arg < 0) return -1;
    return 0;
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

PointRowCol& operator+=(PointRowCol& lhs, const PointRowCol& rhs)
{
    lhs.row = lhs.row + rhs.row;
    lhs.col = lhs.col + rhs.col;
    return lhs;
}
PointRowCol operator*(int lhs, const PointRowCol& rhs)
{
    return { lhs * rhs.row, lhs * rhs.col };
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

PointRowCol sign(const PointRowCol& point)
{
    return PointRowCol{ sign(point.row), sign(point.col) };
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
    return abs(lhs.row) + abs(lhs.col);
}

bool inBounds(PointRowCol point, int rows, int cols)
{
    return point.row >= 0 && point.row < rows&& point.col >= 0 && point.col < cols;
}

