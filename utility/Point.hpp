#pragma once

struct Point
{
    int x;
    int y;
};

Point operator+(Point lhs, Point rhs);
Point operator-(Point lhs, Point rhs);
int manhatanAbs(Point lhs);