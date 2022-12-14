#pragma once

#include <vector>

namespace day1
{

std::vector<std::vector<int>> parse();

class Sollution
{
public:
    int getSollution(const std::vector<std::vector<int>>& input);
    int getSollution_part2(const std::vector<std::vector<int>>& input);
};

}  // namespace day1
