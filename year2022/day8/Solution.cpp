#include "Solution.hpp"

#include <sstream>
#include <parsers/parsers.hpp>
#include <utility/Martix.hpp>
#include <utility/RangesUtils.hpp>
namespace day8
{

InputType parse()
{
    return parsers::getFile(2022, 8) | To<InputType>();
}

int Solution::solve(const InputType& in)
{
    Matrix<int> visibilityMatrixWrapper(in.rows_count(), in.cols_count(), 0);

    for (auto row = 0u; row < in.rows_count(); ++row)
    {
        auto max = -1;
        for (auto col = 0u; col < in.cols_count(); ++col)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
        max = -1;
        for (int col = in.cols_count() -1; col >= 0; --col)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
    }
    for (auto col = 0u; col < in.cols_count(); ++col)
    {
        auto max = -1;
        for (auto row = 0u; row < in.rows_count(); ++row)
        {
            visibilityMatrixWrapper[row][col] = visibilityMatrixWrapper[row][col] || (max < in[row][col]);
            max = std::max(in[row][col], max);
        }
        max = -1;
        for (int row = in.rows_count() -1; row >= 0; --row)
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

int getVisibility(const InputType& in , int row, int col)
{
    auto currentHeight = in[row][col];
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    for (int r = row + 1; r < (int)in.rows_count(); ++r)
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

int Solution::solve_part2(const InputType& in)
{
    auto maxVisibility = 0;
    for (auto row = 0u; row < in.rows_count(); ++row)
    {
        for (auto col = 0u; col < in.cols_count(); ++col)
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