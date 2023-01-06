#include "Solution.hpp"
#include <parsers/parsers.hpp>
#include <utility/PointRowColOrientation.hpp>
#include <iostream>
#include <array>
#include <deque>
#include <unordered_set>
#include <cassert>
#include <queue>

namespace day24
{

const char *fileLoc = "day24/input.txt";

Wall parse()
{
    return parsers::parseMatrixOfChars(fileLoc);
}
namespace
{

PointRowCol teleport(const std::vector<std::vector<char>> &map, PointRowCol pow, Dir orientation)
{
    if (orientation == Dir::up)
    {
        for (int row = map.size()-1; row >= 0; row--)
        {
            if (map[row][pow.col] == '.')
            {
                return {row, pow.col};
            }
        }
    }
    if (orientation == Dir::down)
    {
        for (int row = 0; row < (int)map.size(); row++)
        {
            if (map[row][pow.col] == '.')
            {
                return {row, pow.col};
            }
        }
    }
    if (orientation == Dir::left)
    {
        for (int col = map[0].size()-1; col >= 0; col--)
        {
            if (map[pow.row][col] == '.')
            {
                return {pow.row, col};
            }
        }
    }
    if (orientation == Dir::right)
    {
        for (int col = 0; col < (int)map[0].size(); col++)
        {
            if (map[pow.row][col] == '.')
            {
                return {pow.row, col};
            }
        }
    }
    return {};
}

PointRowCol getDirrectionDif(Dir d)
{
    if (d == Dir::up) return UpperPointDiff;
    if (d == Dir::down) return BottomPointDiff;
    if (d == Dir::left) return LeftPointDiff;
    if (d == Dir::right) return RightPointDiff;
    throw 1;
}

}
bool isRock(const std::vector<std::vector<char> > &map, PointRowCol pos)
{
    return map[pos.row][pos.col] == '#';
}

bool isBlank(const std::vector<std::vector<char> > &map, PointRowCol pos)
{
    return pos.row >= 0 && pos.col >= 0 && pos.row < (int)map.size() && pos.col < (int)map[0].size() && map[pos.row][pos.col] == '.';
}

struct Blizards
{
    explicit Blizards(Wall);
    Blizards getNewIteration();
    const Wall& getFullSurface();
private:
    void performIteration();
    Wall wall_;
    std::vector<PointRowColOrientation> blizzardPoints_;
    std::vector<PointRowCol> rocks_;
};

Blizards::Blizards(Wall wall)
    : wall_(wall)
{
    for (int row = 0; row < (int)wall.size(); ++row)
    {
        for (int col = 0; col < (int)wall[row].size(); ++col)
        {
            if (isRock(wall, PointRowCol{row, col}))
            {
                rocks_.emplace_back(row, col);
            }

            if (wall_[row][col] == '<')
            {
                blizzardPoints_.emplace_back(Dir::left, PointRowCol{row, col});
            }
            if (wall_[row][col] == '>')
            {
                blizzardPoints_.emplace_back(Dir::right, PointRowCol{row, col});
            }
            if (wall_[row][col] == '^')
            {
                blizzardPoints_.emplace_back(Dir::up, PointRowCol{row, col});
            }
            if (wall_[row][col] == 'v')
            {
                blizzardPoints_.emplace_back(Dir::down, PointRowCol{row, col});
            }
        }
    }
}

const Wall& Blizards::getFullSurface()
{
    return wall_;
}

Blizards Blizards::getNewIteration()
{
    Blizards newGeneration_(*this);
    newGeneration_.performIteration();
    return newGeneration_;
}

void Blizards::performIteration()
{
    wall_ = std::vector<std::vector<char>>(wall_.size(), std::vector<char>(wall_[0].size(), '.'));

    for (auto&& rock : rocks_)
    {
        wall_[rock.row][rock.col] = '#';
    }
    for (auto&& bp : blizzardPoints_)
    {
        bp.position_ = bp.position_ + getDirrectionDif(bp.orientation_);
        if (isRock(wall_, bp.position_))
        {
            bp.position_ = teleport(wall_, bp.position_, bp.orientation_);
        }
    }

    for (auto&& bp : blizzardPoints_)
    {
        // wall_[bp.position_.row][bp.position_.col] = 'W';
        if (bp.orientation_ == Dir::left)
        {
            wall_[bp.position_.row][bp.position_.col] += 1ull;
        }
        if (bp.orientation_ == Dir::right)
        {
            wall_[bp.position_.row][bp.position_.col] += 2ull;
        }
        if (bp.orientation_ == Dir::up)
        {
            wall_[bp.position_.row][bp.position_.col] += 4ull;
        }
        if (bp.orientation_ == Dir::down)
        {
            wall_[bp.position_.row][bp.position_.col] += 8ull;
        }
    }
}

[[maybe_unused]]
void printBlizards(const Wall& wall)
{
    for (int row = 0; row < (int)wall.size(); ++row)
    {
        for (int col = 0; col < (int)wall[row].size(); ++col)
        {
            std::cout << wall[row][col];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::deque<Blizards> blizards;
const Wall& getBlizard(int iteration)
{
    assert(iteration <= (int)blizards.size());
    if (iteration >= (int)blizards.size())
    {
        blizards.push_back(blizards.back().getNewIteration());
        auto lastWall = blizards.back().getFullSurface();
    }
    return blizards[iteration].getFullSurface();
}

struct RouteEntity
{
    int iteration;
    PointRowCol point;
};

struct RouteEntityWithHeuristic
{
    RouteEntity re;
    int heuristicVal;
};

bool operator==(const RouteEntity& lhs, const RouteEntity& rhs)
{
    return lhs.iteration == rhs.iteration && rhs.point == lhs.point;
}

bool operator<(const RouteEntityWithHeuristic& lhs, const RouteEntityWithHeuristic& rhs)
{
    return lhs.heuristicVal > rhs.heuristicVal;  // hack ...;]
}

namespace
{

int minRouteSoFar = 1000000;

[[maybe_unused]]
int heuristic(PointRowCol point, PointRowCol destination)
{
    return manhatanAbs(destination-point);
}

struct RouteEntityHash
{
    std::size_t operator()(const RouteEntity& p) const noexcept
    {
        return (p.iteration << 20) | (p.point.row << 10) | (p.point.col << 10);
    }
};

[[maybe_unused]]
void printCurrentRoute (const std::vector<RouteEntity>& currentRoute)
{
    for (auto&& item : currentRoute)
    {
        std::cout << "(iter = " << item.iteration << ", " << item.point << ") ";
    }
    std::cout << std::endl;
}

int solveAStarLike(const PointRowCol& startingPoint, const PointRowCol& endPoint, int iterationInit = 0)
{
    std::unordered_set<RouteEntity, RouteEntityHash> visitedPoints = {};
    std::priority_queue<RouteEntityWithHeuristic> routeEntityPrQueue;

    routeEntityPrQueue.push(RouteEntityWithHeuristic{ RouteEntity{iterationInit, startingPoint}, iterationInit + heuristic(startingPoint, endPoint) });
    while (true)
    {
        auto currentNode = routeEntityPrQueue.top();
        routeEntityPrQueue.pop();

        if (visitedPoints.contains(currentNode.re))
        {
            continue;
        }
        visitedPoints.insert(currentNode.re);
        if (currentNode.re.point == endPoint)
        {
            return currentNode.re.iteration;
        }
        auto nextIteration = currentNode.re.iteration + 1;
        const auto nextSurface = getBlizard(nextIteration);
        constexpr auto neighboursDiffs = std::array<PointRowCol, 5>{BottomPointDiff, RightPointDiff, PointRowCol{ 0, 0 }, UpperPointDiff, LeftPointDiff};
        for (const auto& neighbourDiff : neighboursDiffs)
        {
            auto neighbourPoint = currentNode.re.point + neighbourDiff;

            if (isBlank(nextSurface, neighbourPoint))
            {
                routeEntityPrQueue.push(RouteEntityWithHeuristic{ RouteEntity{nextIteration, neighbourPoint}, nextIteration + heuristic(neighbourPoint, endPoint) });
            }
        }
    }
}

}

int Solution::solve(Wall wall)
{
    blizards = {};
    blizards.push_back(Blizards(wall));

    auto startingPoint = PointRowCol{0, 1};
    auto endPoint = PointRowCol{(int)wall.size()-1, (int)wall[0].size() - 2};

    return solveAStarLike({ 0, 1 }, endPoint);
}

int Solution::solve_part2(Wall wall)
{
    blizards = {};
    blizards.push_back(Blizards(wall));

    auto startingPoint = PointRowCol{0, 1};
    auto endPoint = PointRowCol{(int)wall.size()-1, (int)wall[0].size() - 2};

    auto iterationNum = solveAStarLike(startingPoint, endPoint);
    iterationNum = solveAStarLike(endPoint, startingPoint, iterationNum);
    iterationNum = solveAStarLike(startingPoint, endPoint, iterationNum);

    return iterationNum;
}

}  // namespace
