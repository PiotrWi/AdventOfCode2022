#pragma once

#include <optional>
#include <vector>

namespace year_2023::day10
{

struct Node
{
    explicit Node(char c) : symbol(c) {}
    char symbol;
    std::optional<long long> distanceFromStart;
    std::vector <Node*> neighbours;
    std::optional<long long> groupId;
};

struct Input
{
    int startingRow;
    int startingCol;
    std::vector< std::vector<Node> > nodes;
};

using InputType = Input;  

InputType parse();

class Solution
{
public:
    long long solve(InputType& input) const;
    long long solve_part2(InputType& input) const;
};

}  // namespace year_2023::day10
