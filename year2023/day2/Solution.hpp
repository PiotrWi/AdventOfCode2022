#pragma once

#include <vector>

namespace year_2023::day2
{

struct BallsRecordEntity
{
    int redBalls = 0;
    int greenBalls = 0;
    int blueBalls = 0;
};

struct GameRecord
{
    std::vector<BallsRecordEntity> records = {};
};

std::vector<GameRecord> parse();

class Solution
{
public:
    int solve(const std::vector<GameRecord>& input) const;
    int solve_part2(const std::vector<GameRecord>& input) const;
};

}  // namespace namespace year_2023::day2
