#include "Solution.hpp"
#include <vector>
#include <numeric>
#include <parsers/parsers.hpp>

namespace day4
{

const char* fileLoc = "day4/input.txt";

std::vector<TEntityType> parse()
{
    return parsers::parse<TEntityType>(fileLoc);
}

namespace
{

bool isFullyInRange(TEntityType elem)
{
    if (elem.first.first <= elem.second.first && elem.first.second >= elem.second.second)
        return true;
    if (elem.first.first >= elem.second.first && elem.first.second <= elem.second.second)
        return true;
    return false;
}

bool doOverlap(TEntityType elem)
{

    if (elem.first.second >= elem.second.first && elem.first.first <= elem.second.second)
        return true;
    if (elem.second.second >= elem.first.first && elem.second.first <= elem.first.second)
        return true;
    return false;
}

/*
 * 5-7,7-9 overlaps in a single section, 7.
2-8,3-7 overlaps all of the sections 3 through 7.
6-6,4-6 overlaps in a single section, 6.
2-6,4-8 overlaps in sections 4, 5, and 6.
 */

}  // namespace

int Solution::solve(const std::vector<TEntityType>& input)
{
    int out = std::accumulate(std::begin(input), std::end(input), 0, [](auto&& currentSum, auto&& elem)
    {
        return currentSum + isFullyInRange(elem);
    });
    return out;
}

int Solution::solve_part2(const std::vector<TEntityType>& input)
{
    int out = std::accumulate(std::begin(input), std::end(input), 0, [](auto&& currentSum, auto&& elem)
    {
        return currentSum + doOverlap(elem);
    });
    return out;
}


}  // namespace day4
