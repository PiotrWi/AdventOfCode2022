#pragma once

struct PointXY
{
    int x;
    int y;
};

PointXY operator+(PointXY lhs, PointXY rhs);
PointXY operator-(PointXY lhs, PointXY rhs);
int manhatanAbs(PointXY lhs);

struct PointRowCol
{
    int row;
    int col;
};

PointRowCol operator+(PointRowCol lhs, PointRowCol rhs);