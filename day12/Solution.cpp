#include "Solution.hpp"

#include <algorithm>
#include <parsers/parsers.hpp>
#include <utility/Martix.hpp>

namespace day12
{

const char* fileLoc = "day12/input.txt";

std::vector<std::vector<char>> parse()
{
    return parsers::parseMatrixOfChars(fileLoc);
}

namespace
{

std::pair<int, int> findPos(const std::vector<std::vector<char>>& in, char c)
{
    auto matrixWrapper = createMatrixWrapper(in);
    auto pos = std::find(matrixWrapper.begin(), matrixWrapper.end(), c);
    if (pos != matrixWrapper.end())
    {
        pos.toPointRowCol(matrixWrapper.begin());
    }
    throw 1;
}

bool wasVisited(std::pair<int, int> in, const std::vector<std::vector<int>>& costs)
{
    return costs[in.first][in.second] != -1;
}

std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    return {lhs.first + rhs.first, lhs.second+ rhs.second};
}

auto isInMatrix(std::pair<int, int> in, const std::vector<std::vector<int>>& costs)
{
    return in.first >= 0 && in.second >= 0 && in.first < (int)costs.size() and in.second < (int)costs[0].size();
}

auto isAccessible(std::pair<int, int> source,
                  std::pair<int, int> destination,
                  const std::vector<std::vector<char>>& heights)
{
    return heights[destination.first][destination.second] - heights[source.first][source.second] <= 1;
}

auto is_a(std::pair<int, int> in, const std::vector<std::vector<char>>& heights)
{
    return heights[in.first][in.second] == 'a';
}

auto isAccessible_part2(std::pair<int, int> source,
                  std::pair<int, int> destination,
                  const std::vector<std::vector<char>>& heights)
{
    return heights[source.first][source.second] - heights[destination.first][destination.second] <= 1;
}

}

int Solution::solve(std::vector<std::vector<char>> in)
{
    std::vector<std::vector<int>> stepsToVisit(in.size(), std::vector<int>(in[0].size(), -1));

    std::pair<int, int> startPos = findPos(in, 'S');
    std::pair<int, int> endPos = findPos(in, 'E');
    in[startPos.first][startPos.second] = 'a';
    in[endPos.first][endPos.second] = 'z';

    std::vector<std::pair<int, std::pair<int, int>>> nodesToCheck = {{0, startPos}};
    for (unsigned i = 0; i < nodesToCheck.size(); ++i)
    {
        // need to be copied because we add nodes in the loop
        auto nodeCost = nodesToCheck[i].first;
        auto analyzedNode = nodesToCheck[i].second;

        if (wasVisited(nodesToCheck[i].second, stepsToVisit))
        {
            continue;
        }
        stepsToVisit[analyzedNode.first][analyzedNode.second] = nodeCost;
        if (analyzedNode == endPos)
        {
            return nodeCost;
        }
        for (std::pair<int, int> diff: {
                std::pair<int, int>{1,0},
                std::pair<int, int>{-1, 0},
                std::pair<int, int>{0, 1},
                std::pair<int, int>{0, -1}
        })
        {
            auto neigbourNode = nodesToCheck[i].second + diff;
            if (isInMatrix(neigbourNode, stepsToVisit)
                && isAccessible(analyzedNode, neigbourNode, in))
            {
                nodesToCheck.push_back({nodeCost+1, neigbourNode});
            }
        }
    }
    return 0;
}

int Solution::solve_part2(std::vector<std::vector<char>> in)
{
    std::vector<std::vector<int>> stepsToVisit(in.size(), std::vector<int>(in[0].size(), -1));

    std::pair<int, int> startPos = findPos(in, 'S');
    std::pair<int, int> endPos = findPos(in, 'E');
    in[startPos.first][startPos.second] = 'a';
    in[endPos.first][endPos.second] = 'z';

    startPos = endPos;

    std::vector<std::pair<int, std::pair<int, int>>> nodesToCheck = {{0, startPos}};
    for (unsigned i = 0; i < nodesToCheck.size(); ++i)
    {
        // need to be copied because we add nodes in the loop
        auto nodeCost = nodesToCheck[i].first;
        auto analyzedNode = nodesToCheck[i].second;

        if (wasVisited(nodesToCheck[i].second, stepsToVisit))
        {
            continue;
        }
        stepsToVisit[analyzedNode.first][analyzedNode.second] = nodeCost;
        if (is_a(analyzedNode, in))
        {
            return nodeCost;
        }
        for (std::pair<int, int> diff: {
                std::pair<int, int>{1,0},
                std::pair<int, int>{-1, 0},
                std::pair<int, int>{0, 1},
                std::pair<int, int>{0, -1}
        })
        {
            auto neigbourNode = nodesToCheck[i].second + diff;
            if (isInMatrix(neigbourNode, stepsToVisit)
                && isAccessible_part2(analyzedNode, neigbourNode, in))
            {
                nodesToCheck.push_back({nodeCost+1, neigbourNode});
            }
        }
    }
    return 0;
}

}  // namespace day12
