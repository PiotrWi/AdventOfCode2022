#pragma once

#include "PointRowCol.hpp"

enum class Dir
{
    right = 0,
    down = 1,
    left = 2,
    up = 3,
};

Dir rotateRight(const Dir pos);
Dir rotateLeft(const Dir pos);

struct PointRowColOrientation
{
    Dir orientation_;
    PointRowCol position_;
};

std::ostream& operator<<(std::ostream& str, Dir d);
std::ostream& operator<<(std::ostream& str, PointRowColOrientation position);


