#pragma once

#include <vector>
#include <memory>

namespace day13
{

struct Node
{
public:
    virtual ~Node() = default;
};

struct Integer: public Node
{
    Integer() {}
    Integer(int in): val(in) {}
    int val;
};

struct List: public Node
{
    std::vector<std::unique_ptr<Node>> children;
};

std::vector<std::unique_ptr<Node>> parse();

struct Solution
{
    int solve(const std::vector<std::unique_ptr<Node>>& in);
    int solve_part2(std::vector<std::unique_ptr<Node>>& in);
};

}  // namepsace day13
