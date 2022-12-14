#pragma once

#include <vector>
#include <utility>

namespace day2
{

std::vector<std::pair<char, char>> parse();

class Sollution
{
public:
    int getSollution(const std::vector<std::pair<char, char>>& input);
    int getSollution_part2(const std::vector<std::pair<char, char>>& input);
};

}
