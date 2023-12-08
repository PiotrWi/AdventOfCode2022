#pragma once

#include <string>
#include <vector>

namespace year_2023::day8
{

enum class Side
{
    left = 0,
    Right = 1,
};

struct SingleNode
{
    std::string id;
    std::string neighboursIds[2];
    SingleNode* neighboursNodes[2];
    std::string getLeftId() { return neighboursIds[0]; };
    std::string getRightId() { return neighboursIds[1]; };
    SingleNode*& getLeftNode() { return neighboursNodes[0]; };
    SingleNode*& getRightNode() { return neighboursNodes[1]; };
};

struct Input
{
    std::vector<Side> sides;
    std::vector<SingleNode> nodes;
};

Input parse();

class Solution
{
public:
    long long solve(Input& input) const;
    long long solve_part2(Input& input) const;
};

}  // namespace year_2023::day8
