#include "Solution.hpp"

#include <sstream>
#include <parsers/parsers.hpp>
#include <utility/Martix.hpp>

namespace parsers
{

template<>
auto toT<std::vector<int>>(const std::string& line) -> std::vector<int>
{
    std::vector<int> singleRow;
    std::stringstream ss(line);
    char c;
    while (ss.get(c))
    {
        singleRow.push_back(c);
    }
    return singleRow;
}

}

const char* fileLoc = "year2022/day8/input.txt";

namespace day8
{

std::vector<std::vector<int>> parse()
{
    return parsers::parse<std::vector<int>>(fileLoc);
}

int Solution::solve(const std::vector<std::vector<int>>& in)
{
    std::vector<std::vector<int>> visibility(in.size(), std::vector<int>(in[0].size(), 0));
    auto visibilityMatrixWrapper = createMatrixWrapper(visibility);

    for (auto row = 0u; row < in.size(); ++row)
    {
        auto max = -1;
        for (auto col = 0u; col < in[row].size(); ++col)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
        max = -1;
        for (int col = in[row].size() -1; col >= 0; --col)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
    }
    for (auto col = 0u; col < in[0].size(); ++col)
    {
        auto max = -1;
        for (auto row = 0u; row < in.size(); ++row)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
        max = -1;
        for (int row = in.size() -1; row >= 0; --row)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
    }

    int out = 0;
    for (auto&& elem : visibilityMatrixWrapper)
    {
        out += elem;
    }

    return out;
}

namespace
{

int getVisibility(const std::vector<std::vector<int>>& in , int row, int col)
{
    auto currentHeight = in[row][col];
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    for (int r = row + 1; r < (int)in.size(); ++r)
    {
        up += 1;
        if (in[r][col] >= currentHeight) break;

    }
    for (int r = row - 1; r >= 0; --r)
    {
        down += 1;
        if (in[r][col] >= currentHeight) break;
    }

    for (int c = col + 1; c < (int)in[0].size(); ++c)
    {
        right += 1;
        if (in[row][c] >= currentHeight) break;
    }
    for (int c = col - 1; c >= 0; --c)
    {
        left += 1;
        if (in[row][c] >= currentHeight) break;
    }
    return up*down*left*right;
}

}  // namespace

int Solution::solve_part2(const std::vector<std::vector<int>>& in)
{
    auto maxVisibility = 0;
    for (auto row = 0u; row < in.size(); ++row)
    {
        for (auto col = 0u; col < in[0].size(); ++col)
        {
            auto currentVisibility = getVisibility(in, row, col);
            if (currentVisibility > maxVisibility)
            {
                maxVisibility = currentVisibility;
            }
        }
    }
    return maxVisibility;
}

}  // namespace