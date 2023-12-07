#pragma once

#include <string>
#include <vector>

namespace year_2023::day7
{

struct Card
{
    int value;
};

struct Hand
{
    std::string line;
    Card cards[5];
    int bet;
};

std::vector<Hand> parse();

class Solution
{
public:
    long long solve(const std::vector<Hand>& input) const;
    long long solve_part2(const std::vector<Hand>& input) const;
};

}  // namespace year_2023::day7
