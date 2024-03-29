#pragma once

#include <vector>
#include <string>
#include <set>

namespace year_2023::day5
{

struct Ranges
{
    long long sourceBegin;
    long long destinationBegin;
    long long length;
};

struct Context
{
    std::vector <long long> seeds;
    std::vector<Ranges> seedRanges;
    std::vector< std::vector<Ranges> > ranges;
};

Context parse();

class Solution
{
public:
    long long solve(const Context& input) const;
    long long solve_part2(Context& input) const;
};

}  // namespace year_2023::day5
