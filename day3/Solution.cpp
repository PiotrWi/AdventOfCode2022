#include "Solution.hpp"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <parsers/parsers.hpp>

namespace parsers
{
template<>
inline auto toT<std::string>(const std::string& in) -> std::string
{
    return in;
}
}

namespace day3
{
const char* fileLoc = "day3/input.txt";

std::vector<std::string> parse()
{
    return parsers::parse<std::string>(fileLoc);
}

namespace
{

int getSumOfPriorities(const std::string &in) noexcept
{
    return std::accumulate(in.cbegin(), in.end(), 0, [](auto&& a, auto&& b) {
        if (islower(b))
            return a + b - 'a' + 1;
        return a + b - 'A' + 27;
    });
}

auto getIterToSecondRucstack(std::string& rucstackItems) noexcept
{
    return std::begin(rucstackItems) + rucstackItems.size() / 2;
}

auto sortRuckstackElements(std::string& rucstackItems, std::string::iterator firtsRuctstackEnd) noexcept
{
    std::sort(std::begin(rucstackItems), firtsRuctstackEnd);
    std::sort(firtsRuctstackEnd, std::end(rucstackItems));
}

auto getAllUniqueElementsRepeatedInRanges(std::string::iterator lhsRuckstackBegin, std::string::iterator lhsRuckstackEnd,
    std::string::iterator rhsRuckstackBegin, std::string::iterator rhsRuckstackEnd)
{
    std::string commonPart;
    std::set_intersection(lhsRuckstackBegin, lhsRuckstackEnd,
        rhsRuckstackBegin, rhsRuckstackEnd,
        std::inserter(commonPart, commonPart.end()));
    std::sort(commonPart.begin(), commonPart.end());
    auto firstRepeatedIt = std::unique(commonPart.begin(), commonPart.end());
    commonPart.erase(firstRepeatedIt);
    return commonPart;
}

}  // namespace

int Solution::solve(std::vector<std::string> in) const
{
    return std::accumulate(in.begin(), in.end(), 0, [](auto&& summarized, auto&& ruckstucks) {
        auto firsRuckstuckEnd = getIterToSecondRucstack(ruckstucks);
        sortRuckstackElements(ruckstucks, firsRuckstuckEnd);

        std::string commonPart = getAllUniqueElementsRepeatedInRanges(ruckstucks.begin(), firsRuckstuckEnd, firsRuckstuckEnd, ruckstucks.end());

        return summarized + getSumOfPriorities(commonPart);
    });
}

int Solution::solve_part2(std::vector<std::string> in) const
{
    int out = 0;

    for (unsigned i = 0; i < in.size(); i+=3)
    {
        for (unsigned elfNum = i; elfNum < i+3; elfNum++)
        {
            std::sort(in[elfNum].begin(), in[elfNum].end());
        }
        std::string commonPart = getAllUniqueElementsRepeatedInRanges(std::begin(in[i]), std::end(in[i]),
            std::begin(in[i + 1]), std::end(in[i + 1]));
        std::string commonPartOfGroup = getAllUniqueElementsRepeatedInRanges(std::begin(commonPart), commonPart.end(),
            std::begin(in[i + 2]), std::end(in[i + 2]));

        out += getSumOfPriorities(commonPartOfGroup);
    }
    return out;
}

}  // namespace day3
