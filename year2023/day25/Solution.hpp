#pragma once

#include <vector>
#include <string>

namespace year_2023::day25
{

class Node
{
public:
    std::string name_;
    int index_;
    std::vector<std::string> neighbours_;
    std::vector<int> neighboursIndexes_;
};

using InputType = std::vector<Node>;

InputType parse();

class Solution
{
public:
    long long solve(const InputType& input) const;
    long long solve_part2(const InputType& input) const;
};

}  // namespace year_2023::day25
