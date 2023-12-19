#pragma once

#include <ostream>
#include <functional>

struct PointRowCol
{
    int row;
    int col;
};

PointRowCol operator+(const PointRowCol& lhs, const PointRowCol& rhs);
PointRowCol operator-(const PointRowCol& lhs, const PointRowCol& rhs);

PointRowCol& operator+=(PointRowCol& lhs, const PointRowCol& rhs);
PointRowCol operator*(int lhs, const PointRowCol& rhs);

bool operator==(const PointRowCol& lhs, const PointRowCol &rhs);
bool operator<(const PointRowCol& lhs, const PointRowCol& rhs);
std::ostream& operator << (std::ostream& str, const PointRowCol&);
PointRowCol sign(const PointRowCol& point);

PointRowCol to1BaseIndexing(PointRowCol);
int manhatanAbs(PointRowCol lhs);

template<>
struct std::hash<PointRowCol>
{
    std::size_t operator()(const PointRowCol& point) const noexcept
    {
        /*std::size_t h1 = std::hash<int>{}(point.row);
        std::size_t h2 = std::hash<int>{}(point.col);
        return h1 ^ (h2 << 1);*/
        return (long)point.row | ((long)point.col << 16);
    }
};

constexpr PointRowCol UpperPointDiff{-1, 0};
constexpr PointRowCol BottomPointDiff{1, 0};
constexpr PointRowCol RightPointDiff{0, 1};
constexpr PointRowCol LeftPointDiff{0, -1};

constexpr PointRowCol UpperLeftPointDiff{-1, -1};
constexpr PointRowCol UpperRightPointDiff{-1, 1};
constexpr PointRowCol BottomLeftPointDiff{1, -1};
constexpr PointRowCol BottomRightPointDiff{1, 1};

bool inBounds(PointRowCol point, int rows, int cols);
