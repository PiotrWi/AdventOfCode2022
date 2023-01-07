#include "Solution.hpp"
#include <algorithm>
#include <numeric>
#include <parsers/parsers.hpp>

namespace day1
{

const char* fileLoc = "day1/input.txt";

std::vector<std::vector<int>> parse()
{
    std::vector<std::vector<int>> out;
    std::vector<int> singleInput;
    for (auto&& line : parsers::LinesInFileRange(fileLoc))
    {
        if (not line.empty())
        {
            singleInput.emplace_back(std::stoi(line));
        }
        else
        {
            out.emplace_back(std::move(singleInput));
            singleInput = {};
        }
    }

    return out;
}

namespace
{

std::vector<int> accumulteInputs(const std::vector<std::vector<int>>& input)
{
    std::vector<int> out(input.size(), 0);

    for (unsigned int i = 0; i < input.size(); ++i)
    {
        out[i] = std::accumulate(input[i].begin(), input[i].end(), 0);
    }
    return out;
}

}

int Solution::solve(const std::vector<std::vector<int>>& input) const
{
    auto accumulatedGroups = accumulteInputs(input);
    return *std::max_element(accumulatedGroups.begin(), accumulatedGroups.end());
}

int Solution::solve_part2(const std::vector<std::vector<int>>& input) const
{
    auto accumulatedGroups = accumulteInputs(input);
    std::sort(accumulatedGroups.begin(), accumulatedGroups.end());

    return *accumulatedGroups.rbegin() + *(accumulatedGroups.rbegin() + 1) + *(accumulatedGroups.rbegin()+2);
}

}
