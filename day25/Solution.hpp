#pragma once

#include <string>
#include <vector>

namespace day25
{

std::vector<std::string> parse();

struct Solution
{
    std::string solve(std::vector<std::string>);
    std::string solve_part2(std::vector<std::string>) { return {}; };

};

}  // namespace day25
