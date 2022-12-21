#include "Solution.hpp"

#include <map>
#include <list>
#include <set>
#include <fstream>
#include <StringAlgorithms/StringAlgorithms.hpp>
#include <ranges>
#include <algorithm>

namespace day18
{

const char *fileLoc = "day18/input.txt";

std::vector<Point3d> parse()
{
    std::vector<Point3d> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    while (inputFile)
    {
        std::getline(inputFile, line);
        if (line == "")
        {
            continue;
        }
        auto splited = splitAndTrim(line, ',');
        out.emplace_back(std::stoi(splited[0]), std::stoi(splited[1]), std::stoi(splited[2]));
    }

    return out;
}

namespace
{

int calculateAdjustment(std::map<std::pair<int, int>, std::set<int>> in)
{
    int counter = 0;
    for (auto&& elem: in)
    {
        auto allCubesInLine = elem.second;
        for (auto prev = allCubesInLine.begin(), next = ++allCubesInLine.begin();
            next != allCubesInLine.end();
            prev++, next++)
        {
            if (*next - *prev== 1)
            {
                counter += 1;
            }
        }
    }
    return 2*counter;
}

}  // namespace

int Solution::solve(std::vector<Point3d> points)
{
    std::map<std::pair<int, int>, std::set<int>> groupedByXY;
    std::map<std::pair<int, int>, std::set<int>> groupedByXZ;
    std::map<std::pair<int, int>, std::set<int>> groupedByYZ;

    for (auto&& point : points)
    {
        groupedByXY[std::make_pair(point.x, point.y)].insert(point.z);
        groupedByXZ[std::make_pair(point.x, point.z)].insert(point.y);
        groupedByYZ[std::make_pair(point.y, point.z)].insert(point.x);
    }

    return points.size()*6 - calculateAdjustment(groupedByXY) - calculateAdjustment(groupedByXZ) - calculateAdjustment(groupedByYZ);
}

namespace
{
Point3d operator+(Point3d lhs, Point3d rhs)
{
    return {lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z};
}

Point3d neighbours[6] = {
        {1, 0,0},
        {-1,0,0},
        {0,1,0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1},
};

bool isInBound(std::vector<std::vector<std::vector<char>>> vec, Point3d point)
{
    return point.x >= 0 && point.x < (int)vec.size()
        && point.y >= 0 && point.y < (int)vec[0].size()
        && point.z >= 0 && point.z < (int)vec[0][0].size();
}

void makeRooksForUnaccessible(std::vector<Point3d>& points)
{
    auto maxX = 0;
    auto maxY = 0;
    auto maxZ = 0;

    for (auto&& point : points)
    {
        maxX = std::max(point.x, maxX);
        maxY = std::max(point.y, maxY);
        maxZ = std::max(point.z, maxZ);
    }

    std::vector<std::vector<std::vector<char>>> accessability(maxX+2, std::vector<std::vector<char>>(maxY+2, std::vector<char>(maxZ + 2, ' ' )));
    for (auto&& point : points)
    {
        accessability[point.x][point.y][point.z] = '#';
    }

    // BFS
    std::list<Point3d> toVisit;
    toVisit.push_back(Point3d{0, 0, 0});
    for (auto it = toVisit.begin(); it != toVisit.end(); ++it)
    {
        const auto& point = *it;
        if (accessability[point.x][point.y][point.z] != ' ')
        {
            continue;
        }

        accessability[point.x][point.y][point.z] = 'm';
        for (auto neighbour : neighbours)
        {
            auto destination = point + neighbour;
            if (isInBound(accessability, destination)
                && accessability[destination.x][destination.y][destination.z] == ' ')
            {
                toVisit.push_back(destination);
            }
        }
    }

    // fill
    for (int x = 0; x < (int)accessability.size(); ++x)
    {
        for (int y = 0; y < (int)accessability[x].size(); ++y)
        {
            for (int z = 0; z < (int)accessability[x][y].size(); ++z)
            {
                if (accessability[x][y][z] == ' ')
                {
                    points.push_back({x, y, z});
                }
            }
        }
    }
}

}

int Solution::solve_part2(std::vector<Point3d> points)
{
    makeRooksForUnaccessible(points);

    std::map<std::pair<int, int>, std::set<int>> groupedByXY;
    std::map<std::pair<int, int>, std::set<int>> groupedByXZ;
    std::map<std::pair<int, int>, std::set<int>> groupedByYZ;

    for (auto&& point : points)
    {
        groupedByXY[std::make_pair(point.x, point.y)].insert(point.z);
        groupedByXZ[std::make_pair(point.x, point.z)].insert(point.y);
        groupedByYZ[std::make_pair(point.y, point.z)].insert(point.x);
    }

    return points.size()*6 - calculateAdjustment(groupedByXY) - calculateAdjustment(groupedByXZ) - calculateAdjustment(groupedByYZ);
}


}  // namespace day18
