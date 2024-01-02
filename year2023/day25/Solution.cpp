#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <map>
#include <queue>
#include <set>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

#include <iostream>

namespace year_2023::day25
{

InputType parse()
{
    std::vector<Node> allNodes;
    std::map<std::string, int> idToIndex;
    for (auto &&line: parsers::getFile(2023, 25)) {
        auto nodeToNeighbours = splitAndTrim(line, ':');
        auto nodeStrs = nodeToNeighbours[0];
        auto neighboursStrs = splitAndTrim(nodeToNeighbours[1], ' ');

        Node node;
        node.name_ = nodeStrs;
        node.index_ = allNodes.size();
        node.neighbours_ = neighboursStrs;
        idToIndex[node.name_] = node.index_;
        allNodes.push_back(node);
    }

    std::vector<Node> notCreated;
    for (auto &&createdNode: allNodes)
    {
        for (auto neighbourStr: createdNode.neighbours_)
        {
            if (not idToIndex.contains(neighbourStr))
            {
                Node node;
                node.name_ = neighbourStr;
                node.index_ = allNodes.size() + notCreated.size();
                idToIndex[node.name_] = node.index_;
                notCreated.push_back(node);
            }
            if (idToIndex[neighbourStr] >= (int)allNodes.size())
            {
                auto &neighbourNode = notCreated[idToIndex[neighbourStr] - allNodes.size()];
                if (std::ranges::find(neighbourNode.neighbours_, createdNode.name_) == neighbourNode.neighbours_.end()) {
                    neighbourNode.neighbours_.push_back(createdNode.name_);
                }
            }
            else
            {
                auto &neighbourNode = allNodes[idToIndex[neighbourStr]];
                if (std::ranges::find(neighbourNode.neighbours_, createdNode.name_) == neighbourNode.neighbours_.end()) {
                    neighbourNode.neighbours_.push_back(createdNode.name_);
                }
            }
        }
    }
    std::ranges::copy(notCreated, std::back_inserter(allNodes));

    for (auto &createdNode: allNodes)
    {
        for (auto neighbourStr: createdNode.neighbours_)
        {
            createdNode.neighboursIndexes_.push_back(idToIndex[neighbourStr]);
        }
    }

    return allNodes;
}
struct Edge
{
    int begin;
    int end;
};
bool operator==(Edge lhs, Edge rhs)
{
    return lhs.begin == rhs.begin && lhs.end == rhs.end;
}

std::vector<Edge> minSpanTree(const InputType &input)
{
    std::vector<Edge> wholeTree;
    std::set<int> visited;
    std::queue<Edge> to_visit;

    to_visit.push({0, 0});

    while(wholeTree.size() != input.size() -1 && not to_visit.empty())
    {
        auto edge = to_visit.front();
        to_visit.pop();
        if (visited.contains(edge.end))
        {
            continue;
        }
        visited.insert(edge.end);
        if (edge != Edge{0,0})
        {
            wholeTree.push_back(edge);
        }
        auto& node = input[edge.end];
        for (auto neigbourIndex : node.neighboursIndexes_)
        {
            to_visit.push({edge.end, neigbourIndex});
        }
    }
    return wholeTree;
}

auto splitGroups(const InputType &input, const std::vector<Edge>& minSpanTree, Edge cutEdge)
{
    std::vector<int> groupIndexes = std::vector<int>(input.size(), 0);
    groupIndexes[cutEdge.begin] = 1;
    groupIndexes[cutEdge.end] = 2;

    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto edge : minSpanTree
                         | std::views::filter([cutEdge](auto edge) {return edge != cutEdge; }))
        {
            if (groupIndexes[edge.begin] == groupIndexes[edge.end])
            {
                continue;
            }
            if (groupIndexes[edge.begin])
            {
                groupIndexes[edge.end] = groupIndexes[edge.begin];
                changed = true;
            }
            if (groupIndexes[edge.end])
            {
                groupIndexes[edge.begin] = groupIndexes[edge.end];
                changed = true;
            }
        }
    }
    return groupIndexes;
}
/*
template <int N>
std::optional< std::vector<int> > tryToSplitGroups(const InputType &input, std::vector<Edge> minSpanTree)
{
    for (auto cutIndex = 0u; cutIndex < minSpanTree.size(); cutIndex++)
    {
        auto cutEdge = minSpanTree[cutIndex];

        std::vector<int> groupIndexes = splitGroups(input, minSpanTree, cutEdge);

        auto sum = 0ull;
        for (auto i = 0u; i < groupIndexes.size(); ++i)
        {
            if (groupIndexes[i] == 1)
            {
                continue;
            }
            for (auto neighbourIndex : input[i].neighboursIndexes_)
            {
                if (groupIndexes[neighbourIndex] == 1)
                {
                    sum += 1;
                }
            }
        }

        if (sum <= N)
        {
            return groupIndexes;
        }
    }
    return {};
}*/

/*
 * This is graphix based solution.
 * I am not very happy about it, but solution is correct.
 */
long long Solution::solve(const InputType &input) const
{
    auto minTree = minSpanTree(input);

    return (input.size() - minTree.size() - 1) * (minTree.size() + 1);
}

long long Solution::solve_part2(const InputType&) const
{
    return 0;
}

}  // namespace year_2023::day25
