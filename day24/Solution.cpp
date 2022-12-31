#include "Solution.hpp"
#include <parsers/parsers.hpp>
#include <utility/PointRowColOrientation.hpp>
#include <iostream>
#include <deque>
#include <unordered_set>
#include <cassert>

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
        wall_[bp.position_.row][bp.position_.col] = 'W';
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
const Wall & getBlizard(int iteration)
{
    assert(iteration <= (int)blizards.size());
    if (iteration >= (int)blizards.size())
    {
        blizards.push_back(blizards.back().getNewIteration());
        // std::cout << "Blizard number: " << iteration << std::endl;
        // printBlizards(blizards.back().getFullSurface());
    }
    return blizards[iteration].getFullSurface();
}

struct RouteEntity
{
    int iteration;
    PointRowCol point;
};

bool operator==(const RouteEntity& lhs, const RouteEntity& rhs)
{
    return lhs.iteration == rhs.iteration && rhs.point == lhs.point;
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

std::unordered_set<RouteEntity, RouteEntityHash> visitedPoints = {};

template <bool reverse>
int visit(RouteEntity point, PointRowCol destination, std::vector<RouteEntity> currentRoute)
{
    if (visitedPoints.contains(point))
    {
        return 1000000;
    }
    visitedPoints.insert(point);

    currentRoute.push_back(point);
    // printCurrentRoute(currentRoute);

    int minRoute = 1000000;
    if (point.point == destination)
    {
        // std::cout << "reached: " << point.iteration << std::endl;
        minRouteSoFar = std::min(minRouteSoFar, point.iteration);
        return point.iteration;
    }

    if (point.iteration + heuristic(point.point, destination) >= minRouteSoFar)
    {
        return 1000000;
    }

    const auto nextSurface = getBlizard(point.iteration+1);
    auto neighboursDiffs = std::vector<PointRowCol>{BottomPointDiff, RightPointDiff, PointRowCol{0, 0}, UpperPointDiff, LeftPointDiff};
    if (reverse)
    {
        std::reverse(neighboursDiffs.begin(), neighboursDiffs.end());
    }
    for (const auto& neighbourDiff : neighboursDiffs)
    {
        auto neighbourPoint = point.point + neighbourDiff;
        if (isBlank(nextSurface, neighbourPoint))
        {
            minRoute = std::min(minRoute, visit<reverse>(RouteEntity{point.iteration + 1, neighbourPoint}, destination, currentRoute));
        }
    }

    minRouteSoFar = std::min(minRouteSoFar, minRoute);

    return minRoute;
}

}

// 288 to high??

int Solution::solve(Wall wall)
{
    minRouteSoFar = 1000000;
    blizards = {};
    blizards.push_back(Blizards(wall));
    auto startingPoint = RouteEntity {0, {0, 1}};
    auto endPoint = PointRowCol{(int)wall.size()-1, (int)wall[0].size() - 2};

    return visit<false>(startingPoint, endPoint, {});
}

int Solution::solve_part2(Wall wall)
{
    minRouteSoFar = 1000000;
    blizards = {};
    blizards.push_back(Blizards(wall));
    // printBlizards(blizards.back().getFullSurface());
    visitedPoints = {};
    auto startingPoint = RouteEntity {0, {0, 1}};
    auto endPoint = PointRowCol{(int)wall.size()-1, (int)wall[0].size() - 2};
    auto iterationNum = visit<false>(startingPoint, endPoint, {});

    std::cout << "First part" << iterationNum << std::endl;

    minRouteSoFar = 1000000;
    visitedPoints = {};
    startingPoint = RouteEntity{iterationNum, {(int)wall.size()-1, (int)wall[0].size() - 2}};
    endPoint =  PointRowCol{0, 1};
    iterationNum = visit<true>(startingPoint, endPoint, {});

    std::cout << "Second part" << iterationNum << std::endl;

    minRouteSoFar = 1000000;
    visitedPoints = {};
    startingPoint = RouteEntity {iterationNum, {0, 1}};
    endPoint = PointRowCol{(int)wall.size()-1, (int)wall[0].size() - 2};
    iterationNum = visit<false>(startingPoint, endPoint, {});

    std::cout << "Third part" << iterationNum << std::endl;

    return iterationNum;
}

}  // namespace
