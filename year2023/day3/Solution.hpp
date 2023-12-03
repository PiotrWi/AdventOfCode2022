#pragma once

#include <vector>
#include <string>

namespace year_2023::day3
{

    std::vector<std::string> parse();

    class Solution
    {
    public:
        long long solve(const std::vector<std::string>& input) const;
        long long solve_part2(const std::vector<std::string>& input) const;
    };

}  // namespace year_2023::day3
