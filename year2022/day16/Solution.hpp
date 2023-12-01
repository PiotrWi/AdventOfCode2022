#pragma once

#include <string>
#include <map>
#include <vector>

namespace day16
{

struct Node
{
    std::string label;
    unsigned char flowRate;
    std::vector<std::string> neighbours;
};

std::map<std::string, Node> parse();

struct Solution
{
    int solve(std::map<std::string, Node>);
    int solve_part2(std::map<std::string, Node>);
};

}  // namespace day16
