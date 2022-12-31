#include "PointRowColOrientation.hpp"

std::ostream& operator<<(std::ostream& str, Dir d)
{
    if (d == Dir::right)
    {
        return str << "right";
    }
    if (d == Dir::left)
    {
        return str << "left";
    }
    if (d == Dir::up)
    {
        return str << "up";
    }
    if (d == Dir::down)
    {
        return str << "down";
    }
    throw 1;
}

std::ostream& operator<<(std::ostream& str, PointRowColOrientation position)
{
    return str << "pos = " << position.position_ << " " << position.orientation_;
}


Dir rotateRight(const Dir pos)
{
    auto dirInt = static_cast<int>(pos);
    dirInt = (dirInt + 1) % 4;
    return static_cast<Dir>(dirInt);
}

Dir rotateLeft(const Dir pos)
{
    auto dirInt = static_cast<int>(pos);
    dirInt = (dirInt - 1 + 4) % 4;
    return static_cast<Dir>(dirInt);
}
