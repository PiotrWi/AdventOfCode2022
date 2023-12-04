#pragma once

#include <vector>
#include <string>
#include <set>

namespace year_2023::day4
{

struct Game
{
    std::set<int> winningNumbers_;
    std::set<int> myNumbers_;
};

std::vector<Game> parse();

class Solution
{
public:
    long long solve(const std::vector<Game>& input) const;
    long long solve_part2(const std::vector<Game>& input) const;
};

}  // namespace year_2023::day4
