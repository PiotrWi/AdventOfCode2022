#include "Solution.hpp"

#include <algorithm>
#include <parsers/parsers.hpp>
#include <utility/Martix.hpp>
#include <iostream>
#include <functional>
#include <utility/RangesUtils.hpp>

namespace day12
{

    InputType parse()
{
    return parsers::getFile(2023, 23) | To<InputType>();
}

namespace
{

PointRowCol findPos(const InputType& in, char c)
{
    auto pos = std::find(in.begin(), in.end(), c);
    if (pos != in.end())
    {
        return pos.getPoint();
    }

    throw 1;
}

bool wasVisited(PointRowCol in, const Matrix<int>& costs)
{
    return costs[in] != -1;
}

auto isAccessible(PointRowCol source,
    PointRowCol destination,
    const InputType& heights)
{
    return heights[destination] - heights[source] <= 1;
}

auto is_a(PointRowCol in, const InputType& heights)
{
    return heights[in] == 'a';
}

auto isAccessible_part2(PointRowCol source,
                    PointRowCol destination,
                  const InputType& heights)
{
    return heights[source] - heights[destination] <= 1;
}

struct BFS
{
public:
    BFS(Matrix<int>& costMatrix)
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
                if (costMatrix_.inBounds(neigbourNode)
                    && isValidTransaction(analyzedNode, neigbourNode))
                {
                    nodesToCheck.push_back({ nodeCost + 1, neigbourNode });
                }
            }
        }
        return -1;
    }
private:
    Matrix<int> costMatrix_;
};

}

int Solution::solve(InputType in)
{
    Matrix<int> costMatrix(in.rows_count(), in[0].size(), -1);

    auto startPos = findPos(in, 'S');
    auto endPos = findPos(in, 'E');
    in[startPos.row][startPos.col] = 'a';
    in[endPos.row][endPos.col] = 'z';

    return BFS(costMatrix).calculateTotalCost(startPos,
        [&endPos](auto&& point) {return point == endPos; },
        [&](auto&& sourceNode, auto&& destinationNode)
        {
            return isAccessible(sourceNode, destinationNode, in);
        });
    return 0;
}

int Solution::solve_part2(InputType in)
{
    Matrix<int> costMatrix(in.rows_count(), in[0].size(), -1);

    auto startPos = findPos(in, 'S');
    auto endPos = findPos(in, 'E');
    in[startPos.row][startPos.col] = 'a';
    in[endPos.row][endPos.col] = 'z';
    startPos = endPos;

    return BFS(costMatrix).calculateTotalCost(startPos,
        [&](auto&& point) {return is_a(point, in); },
        [&](auto&& sourceNode, auto&& destinationNode)
        {
            return isAccessible_part2(sourceNode, destinationNode, in);
        });
}

}  // namespace day12
