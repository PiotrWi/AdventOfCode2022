#pragma once

#include <vector>


namespace year_2023::day6
{

struct SingleRace
{
    long long timeToSail;
    long long distanceToMake;
};

std::vector<SingleRace> parse();

class Solution
{
public:
    long long solve(const std::vector<SingleRace>& input) const;
    long long solve_part2(const std::vector<SingleRace>& input) const;
};

}  // namespace year_2023::day6
