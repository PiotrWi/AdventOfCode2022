#include "Solution.hpp"

#include <algorithm>
#include <parsers/parsers.hpp>
#include <utility/Martix.hpp>
#include <iostream>
#include <functional>

namespace day12
{

const char* fileLoc = "year2022/day12/input.txt";

std::vector<std::vector<char>> parse()
{
    return parsers::parseMatrixOfChars(fileLoc);
}

namespace
{

PointRowCol findPos(const std::vector<std::vector<char>>& in, char c)
{
    auto matrixWrapper = createMatrixWrapper(in);
    auto pos = std::find(matrixWrapper.begin(), matrixWrapper.end(), c);
    if (pos != matrixWrapper.end())
    {
        return pos.toPointRowCol(matrixWrapper.begin());
    }

    throw 1;
}

bool wasVisited(PointRowCol in, const MatrixWrapper<int>& costs)
{
    return costs[in] != -1;
}

auto isAccessible(PointRowCol source,
    PointRowCol destination,
                  const std::vector<std::vector<char>>& heights)
{
    return heights[destination.row][destination.col] - heights[source.row][source.col] <= 1;
}

auto is_a(PointRowCol in, const std::vector<std::vector<char>>& heights)
{
    return heights[in.row][in.col] == 'a';
}

auto isAccessible_part2(PointRowCol source,
                    PointRowCol destination,
                  const std::vector<std::vector<char>>& heights)
{
    return heights[source.row][source.col] - heights[destination.row][destination.col] <= 1;
}

struct BFS
{
public:
    BFS(MatrixWrapper<int>& costMatrix)
        : costMatrix_(costMatrix)
    {}
    
    int calculateTotalCost(const PointRowCol& startingPoint,
        std::function<bool(PointRowCol)> stopCondition,
        std::function<bool(PointRowCol, PointRowCol)> isValidTransaction)
    {
        std::vector<std::pair<int, PointRowCol>> nodesToCheck = { {0, startingPoint} };
        for (unsigned i = 0; i < nodesToCheck.size(); ++i)
        {
            auto nodeCost = nodesToCheck[i].first;
            auto analyzedNode = nodesToCheck[i].second;

            if (wasVisited(nodesToCheck[i].second, costMatrix_))
            {
                continue;
            }
            costMatrix_[analyzedNode] = nodeCost;
            if (stopCondition(analyzedNode))
            {
                return nodeCost;
            }
            for (auto&& diff : {
                BottomPointDiff,
                UpperPointDiff,
                RightPointDiff,
                LeftPointDiff})
            {
                auto neigbourNode = nodesToCheck[i].second + diff;
                if (costMatrix_.isInBound(neigbourNode)
                    && isValidTransaction(analyzedNode, neigbourNode))
                {
                    nodesToCheck.push_back({ nodeCost + 1, neigbourNode });
                }
            }
        }
        return -1;
    }
private:
    MatrixWrapper<int> costMatrix_;
};

}

int Solution::solve(std::vector<std::vector<char>> in)
{
    std::vector<std::vector<int>> stepsToVisit(in.size(), std::vector<int>(in[0].size(), -1));

    auto startPos = findPos(in, 'S');
    auto endPos = findPos(in, 'E');
    in[startPos.row][startPos.col] = 'a';
    in[endPos.row][endPos.col] = 'z';

    auto costMatrix = createMatrixWrapper(stepsToVisit);
    return BFS(costMatrix).calculateTotalCost(startPos,
        [&endPos](auto&& point) {return point == endPos; },
        [&](auto&& sourceNode, auto&& destinationNode)
        {
            return isAccessible(sourceNode, destinationNode, in);
        });
    return 0;
}

int Solution::solve_part2(std::vector<std::vector<char>> in)
{
    std::vector<std::vector<int>> stepsToVisit(in.size(), std::vector<int>(in[0].size(), -1));

    auto startPos = findPos(in, 'S');
    auto endPos = findPos(in, 'E');
    in[startPos.row][startPos.col] = 'a';
    in[endPos.row][endPos.col] = 'z';
    startPos = endPos;

    auto costMatrix = createMatrixWrapper(stepsToVisit);
    return BFS(costMatrix).calculateTotalCost(startPos,
        [&](auto&& point) {return is_a(point, in); },
        [&](auto&& sourceNode, auto&& destinationNode)
        {
            return isAccessible_part2(sourceNode, destinationNode, in);
        });
}

}  // namespace day12
