#include "Solution.hpp"
#include <vector>
#include <numeric>
#include <parsers/parsers.hpp>

namespace parsers
{

template <>
inline auto toT<::day4::TEntityType>(const std::string& in) -> ::day4::TEntityType
{
    char c;
    ::day4::TEntityType out;
    std::stringstream lineStream(in);
    lineStream >> out.first.first >> c >> out.first.second >> c >> out.second.first >> c >> out.second.second;
    return out;
}

}

namespace day4
{

std::vector<TEntityType> parse()
{
    const char* fileLoc = "year2022/day4/input.txt";
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

}  // namespace

int Solution::solve(const std::vector<TEntityType>& input) const
{
    int out = std::accumulate(std::begin(input), std::end(input), 0, [](auto&& currentSum, auto&& elem)
    {
        return currentSum + isFullyInRange(elem);
    });
    return out;
}

int Solution::solve_part2(const std::vector<TEntityType>& input) const
{
    int out = std::accumulate(std::begin(input), std::end(input), 0, [](auto&& currentSum, auto&& elem)
    {
        return currentSum + doOverlap(elem);
    });
    return out;
}


}  // namespace day4
