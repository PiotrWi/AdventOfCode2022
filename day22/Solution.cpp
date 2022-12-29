#include "Solution.hpp"

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/PointXY.hpp>

namespace day22
{

const char *fileLoc = "day22/input.txt";

Input parse()
{
    Input out;

    parsers::LinesInFileRange range(fileLoc);

    auto it = range.begin();
    for (; it != range.end() && *it != ""; ++it)
    {
        out.map_.emplace_back(*it);
    }

    ++it;

    auto line = *it;
    auto numAndLeters = splitNumbersAndLetters(line);
    for (auto entity: numAndLeters)
    {
        if (std::isdigit(entity[0]))
        {
            out.command_.emplace_back(Command{CommandType::GoForward, std::stoi(entity)});
            continue;
        }
        if (entity[0] == 'R')
        {
            out.command_.emplace_back(Command{CommandType::RotateRight, {}});
            continue;
        }
        if (entity[0] == 'L')
        {
            out.command_.emplace_back(Command{CommandType::RotateLeft, {}});
            continue;
        }
    }
    return out;
}

namespace
{

PointRowCol findStartingPos(const std::vector<std::string>& map)
{
    return PointRowCol{0, (int)map[0].find('.')};
}

enum class Dir
{
    up,
    down,
    left,
    right,
};

struct Position
{
    Dir orientation_;
    PointRowCol position_;
};

Dir rotateRight(const Dir pos)
{
    if (pos == Dir::up)
    {
        return Dir::right;
    }
    if (pos == Dir::right)
    {
        return Dir::down;
    }
    if (pos == Dir::down)
    {
        return Dir::left;
    }
    if (pos == Dir::left)
    {
        return Dir::up;
    }
    throw 1;
}

Dir rotateLeft(const Dir pos)
{
    if (pos == Dir::up)
    {
        return Dir::left;
    }
    if (pos == Dir::left)
    {
        return Dir::down;
    }
    if (pos == Dir::down)
    {
        return Dir::right;
    }
    if (pos == Dir::right)
    {
        return Dir::up;
    }
    throw 1;
}

PointRowCol getDirrectionDif(Dir d)
{
    PointRowCol up{-1, 0};
    PointRowCol down{1, 0};
    PointRowCol right{0, 1};
    PointRowCol left{0, -1};

    if (d == Dir::up) return up;
    if (d == Dir::down) return down;
    if (d == Dir::left) return left;
    if (d == Dir::right) return right;
    throw 1;
}

PointRowCol goForward(Position pos, int count, const std::vector<std::string>&)
{
    auto diff = getDirrectionDif(pos.orientation_);
    for (int c = 0; c < count; ++c)
    {
        pos.position_ = pos.position_ + diff;
    }
    throw "Not done yet";
}

}  // namespace

int Solution::solve(Input in)
{
    auto position = Position{Dir::right, findStartingPos(in.map_)};

    for (auto&& command : in.command_)
    {
        if (command.type == CommandType::RotateRight)
        {
            position.orientation_ =  rotateRight(position.orientation_);
            continue;
        }
        if (command.type == CommandType::RotateLeft)
        {
            position.orientation_ =  rotateLeft(position.orientation_);
            continue;
        }
        position.position_ = goForward(position, command.count, in.map_);
    }
    throw "Not done yet";
}


}  // namespace day22
