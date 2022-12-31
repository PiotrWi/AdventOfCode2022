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

int Solution::solve(const std::vector<std::vector<int>>& input)
{
    auto max = 0;
    for (unsigned int i = 0; i < input.size(); ++i)
    {
        auto current = std::accumulate(input[i].begin(), input[i].end(), 0);
        max = std::max(current, max);
    }
    return max;
}

// Not the optimal one, but short to code.
int Solution::solve_part2(const std::vector<std::vector<int>>& input)
{
    std::vector<int> out(input.size(), 0);

    for (unsigned int i = 0; i < input.size(); ++i)
    {
        out[i] = std::accumulate(input[i].begin(), input[i].end(), 0);
    }
    std::sort(out.begin(), out.end());

    return *out.rbegin() + *(out.rbegin() + 1) + *(out.rbegin()+2);
}

}
