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

bool operator==(const PointRowCol& lhs, const PointRowCol &rhs);
bool operator<(const PointRowCol& lhs, const PointRowCol& rhs);
std::ostream& operator << (std::ostream& str, const PointRowCol&);

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
        return (long)point.row | ((long)point.col << 32);
    }
};

inline PointRowCol UpperPointDiff{-1, 0};
inline PointRowCol BottomPointDiff{1, 0};
inline PointRowCol RightPointDiff{0, 1};
inline PointRowCol LeftPointDiff{0, -1};

inline PointRowCol UpperLeftPointDiff{-1, -1};
inline PointRowCol UpperRightPointDiff{-1, 1};
inline PointRowCol BottomLeftPointDiff{1, -1};
inline PointRowCol BottomRightPointDiff{1, 1};