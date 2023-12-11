#include "Solution.hpp"

#include <algorithm>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <utility/PointRowColOrientation.hpp>

namespace day22
{

Input parse()
{
    Input out;
    auto range = parsers::getFile(2022, 22);

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

PointRowCol findStartingPos(const std::vector<std::string> &map)
{
    return PointRowCol{0, (int) map[0].find('.')};
}

[[maybe_unused]]
std::ostream& operator<<(std::ostream& str, Command c)
{
    if (CommandType::RotateLeft == c.type)
    {
        return  str << "rotate left";
    }
    if (CommandType::RotateRight == c.type)
    {
        return  str << "rotate right";
    }
    if (CommandType::GoForward == c.type)
    {
        return  str << "go forward, c = " << c.count;
    }
    throw 1;
}

bool isOutside(const std::vector<std::string> &map, PointRowCol pos)
{
    return pos.row < 0
           || pos.col < 0
           || pos.row >= (int) map.size()
           || pos.col >= (int) map[0].size();
}

bool isBlank(const std::vector<std::string> &map, PointRowCol pos)
{
    return map[pos.row][pos.col] == ' ';
}

bool isRock(const std::vector<std::string> &map, PointRowCol pos)
{
    return map[pos.row][pos.col] == '#';
}

PointRowCol teleport(const std::vector<std::string> &map, PointRowCol pow, Dir orientation)
{
    if (orientation == Dir::up)
    {
        for (int row = map.size()-1; row >= 0; row--)
        {
            if (map[row][pow.col] != ' ')
            {
                return {row, pow.col};
            }
        }
    }
    if (orientation == Dir::down)
    {
        for (int row = 0; row < (int)map.size(); row++)
        {
            if (map[row][pow.col] != ' ')
            {
                return {row, pow.col};
            }
        }
    }
    if (orientation == Dir::left)
    {
        for (int col = map[0].size()-1; col >= 0; col--)
        {
            if (map[pow.row][col] != ' ')
            {
                return {pow.row, col};
            }
        }
    }
    if (orientation == Dir::right)
    {
        for (int col = 0; col < (int)map[0].size(); col++)
        {
            if (map[pow.row][col] != ' ')
            {
                return {pow.row, col};
            }
        }
    }
    return {};
}

PointRowCol goForward(PointRowColOrientation pos, int count, const std::vector<std::string>& map)
{
    auto diff = getDirrectionDif(pos.orientation_);
    for (int c = 0; c < count; ++c)
    {
        auto posCadidate = pos.position_ + diff;
        if (isOutside(map, posCadidate) || isBlank(map, posCadidate))
        {
            posCadidate = teleport(map, posCadidate, pos.orientation_);
        }
        if (isRock(map, posCadidate))
        {
            return pos.position_;
        }
        pos.position_ = posCadidate;
    }
    return pos.position_;
}

void alignMapSizeForAllRows(std::vector<std::string>& map)
{
    decltype(map.size()) maxW = 0ul;
    for (auto&& s: map)
    {
        maxW = std::max(maxW, s.size());
    }
    for (auto&& s: map)
    {
        auto trailingBlanks = maxW - s.size();
        s += std::string (trailingBlanks, ' ');
    }
}

}  // namespace

int Solution::solve(Input in)
{
    alignMapSizeForAllRows(in.map_);
    auto position = PointRowColOrientation{Dir::right, findStartingPos(in.map_)};

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

    return 1000ll * (position.position_.row + 1) + 4 * (position.position_.col + 1) + static_cast<int>(position.orientation_);
}

namespace
{

int getFaceNum(PointRowCol pos)
{
    if (pos.row>=0 && pos.row < 50 && pos.col >= 50 && pos.col < 100)
    {
        return 1;
    }
    if (pos.row>=0 && pos.row < 50 && pos.col >= 100 && pos.col < 150)
    {
        return 2;
    }
    if (pos.row >= 50 && pos.row < 100 && pos.col >= 50 && pos.col < 100)
    {
        return 3;
    }
    if (pos.row >= 100 && pos.row < 150 && pos.col >= 50 && pos.col < 100)
    {
        return 4;
    }

    if (pos.row >= 100 && pos.row < 150 && pos.col >= 0 && pos.col < 50)
    {
        return 5;
    }
    if (pos.row >= 150 && pos.row < 200 && pos.col >= 0 && pos.col < 50)
    {
        return 6;
    }
    throw;
}

PointRowColOrientation switchFace(PointRowColOrientation pos)
{
    auto faceNum = getFaceNum(pos.position_);
    if (faceNum == 1)
    {
        if (pos.orientation_ == Dir::up) // to 6
        {
            return PointRowColOrientation{rotateRight(pos.orientation_), PointRowCol{150 + (pos.position_.col - 50), 0} };
        }
        if (pos.orientation_ == Dir::left) // to 5
        {
            return PointRowColOrientation{rotateRight(rotateRight(pos.orientation_)), PointRowCol{149 - (pos.position_.row), 0} };
        }
    }
    if (faceNum == 2)
    {
        if (pos.orientation_ == Dir::up) // to 6
        {
            return PointRowColOrientation{pos.orientation_, PointRowCol{199, pos.position_.col - 100} };
        }
        if (pos.orientation_ == Dir::right) // to 4
        {
            return PointRowColOrientation{rotateRight(rotateRight(pos.orientation_)), PointRowCol{149 - pos.position_.row, 99} };
        }
        if (pos.orientation_ == Dir::down) // to 3
        {
            return PointRowColOrientation{rotateRight(pos.orientation_), PointRowCol{50 + pos.position_.col - 100, 99}};
        }
    }
    if (faceNum == 3) // to 22
    {
        if (pos.orientation_ == Dir::right)
        {
            return PointRowColOrientation{rotateLeft(pos.orientation_), PointRowCol{49, 100 + pos.position_.row - 50}};
        }
        if (pos.orientation_ == Dir::left)
        {
            return PointRowColOrientation{rotateLeft(pos.orientation_), PointRowCol{100, 0 + pos.position_.row - 50}};
        }
    }
    if (faceNum == 4)
    {
        if (pos.orientation_ == Dir::right)  // to 2
        {
            return PointRowColOrientation{rotateLeft(rotateLeft(pos.orientation_)), PointRowCol{49 - (pos.position_.row - 100), 149}};
        }
        if (pos.orientation_ == Dir::down)  // to 6
        {
            return PointRowColOrientation{rotateRight(pos.orientation_), PointRowCol{150 + pos.position_.col - 50, 49}};
        }
    }
    if (faceNum == 5)
    {
        if (pos.orientation_ == Dir::up)  // to 3
        {
            return PointRowColOrientation{rotateRight(pos.orientation_), PointRowCol{50 + pos.position_.col, 50}};
        }
        if (pos.orientation_ == Dir::left)  // to 3
        {
            return PointRowColOrientation{rotateRight(rotateRight(pos.orientation_)), PointRowCol{149 - pos.position_.row, 50}};
        }
    }
    if (faceNum == 6)
    {
        if (pos.orientation_ == Dir::left)
        {
            return PointRowColOrientation{rotateLeft(pos.orientation_), PointRowCol{0, 50 + pos.position_.row - 150}};
        }
        if (pos.orientation_ == Dir::down)
        {
            return PointRowColOrientation{pos.orientation_, PointRowCol{0, 100 + pos.position_.col}};
        }
        if (pos.orientation_ == Dir::right)  // to 4
        {
            return PointRowColOrientation{rotateLeft(pos.orientation_), PointRowCol{149, 50 + pos.position_.row - 150}};
        }
    }
    return {};
}

PointRowColOrientation goForwardOnCube(PointRowColOrientation pos, int count, const std::vector<std::string>& map)
{
    for (int c = 0; c < count; ++c)
    {
        auto diff = getDirrectionDif(pos.orientation_);
        auto posCadidate = PointRowColOrientation{pos.orientation_, pos.position_ + diff, };
        if (isOutside(map, posCadidate.position_) || isBlank(map, posCadidate.position_))
        {
            posCadidate = switchFace(pos);
        }
        if (isRock(map, posCadidate.position_))
        {
            return pos;
        }
        pos = posCadidate;
    }
    return pos;
}

}

int Solution::solve_part2(Input in)
{
    alignMapSizeForAllRows(in.map_);
    auto position = PointRowColOrientation{Dir::right, findStartingPos(in.map_)};

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
        position = goForwardOnCube(position, command.count, in.map_);
    }
    return 1000ll * (position.position_.row + 1) + 4 * (position.position_.col + 1) + static_cast<int>(position.orientation_);
}

}  // namespace day22
