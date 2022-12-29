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

int getSumOfPriorities(std::string &in, std::string ::iterator& end)
{
    return std::accumulate(in.begin(), end, 0, [](auto&& a, auto&&b ) {
        if (islower(b))
            return a + b - 'a' + 1;
        return a + b - 'A' + 27;
    });
}

}  // namespace

int Solution::solve(std::vector<std::string>&& in)
{
    int out = 0;
    for(auto&& elem: in)
    {
        auto firsCompartmentEnd = std::begin(elem) + elem.size() / 2;
        std::sort(std::begin(elem), firsCompartmentEnd);
        std::sort(firsCompartmentEnd, std::end(elem));
        std::string commonPart;
        std::set_intersection(std::begin(elem), firsCompartmentEnd,
                              firsCompartmentEnd, std::end(elem),
                              std::inserter(commonPart, commonPart.end()));
        std::sort(commonPart.begin(), commonPart.end());
        auto firstRepeatedIt = std::unique(commonPart.begin(), commonPart.end());

        out += getSumOfPriorities(commonPart, firstRepeatedIt);
    }
    return out;
}

int Solution::solve_part2(std::vector<std::string>&& in)
{
    int out = 0;

    for (unsigned i = 0; i < in.size(); i+=3)
    {
        for (unsigned elfNum = i; elfNum < i+3; elfNum++)
        {
            std::sort(in[elfNum].begin(), in[elfNum].end());
        }
        std::string commonPart;
        std::set_intersection(std::begin(in[i]), std::end(in[i]),
                              std::begin(in[i+1]), std::end(in[i+1]),
                              std::inserter(commonPart, commonPart.end()));
        std::sort(commonPart.begin(), commonPart.end());
        auto firstRepeated = std::unique(commonPart.begin(), commonPart.end());

        std::string commonPartOfGroup;
        std::set_intersection(std::begin(commonPart), firstRepeated,
                              std::begin(in[i+2]), std::end(in[i+2]),
                              std::inserter(commonPartOfGroup, commonPartOfGroup.end()));
        auto firstRepeatedOfGroup = std::unique(commonPartOfGroup.begin(), commonPartOfGroup.end());

        out += getSumOfPriorities(commonPartOfGroup, firstRepeatedOfGroup);
    }
    return out;
}

}  // namespace day3
