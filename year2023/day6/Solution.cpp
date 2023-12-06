#pragma once

#include <iostream>
#include "Solution.hpp"

namespace year_2023::day6
{

namespace
{

struct Range
{
    long long begin;
    long long end;
};

bool calc(const SingleRace& in, int timePushingButton)
{
    return (in.timeToSail - timePushingButton) * timePushingButton > in.distanceToMake;
}

int findFirstSmallerLeft(Range r, const SingleRace& in)
{
    if (r.end - r.begin <= 1) { return r.begin; }

    auto middle = r.begin + (r.end - r.begin) / 2;
    if (calc(in, middle))
    {
        return findFirstSmallerLeft(Range{ r.begin, middle}, in);
    }
    return findFirstSmallerLeft(Range{ middle, r.end }, in);
}

int lastGreaterRight(Range r, const SingleRace& in)
{
    if (r.end - r.begin <= 1) { return r.begin; }

    auto middle = r.begin + (r.end - r.begin) / 2;
    if (not calc(in, middle))
    {
        return lastGreaterRight(Range{ r.begin, middle }, in);
    }
    return lastGreaterRight(Range{ middle, r.end }, in);
}

int resolve(const SingleRace& in)
{
    Range range1, range2;
    if (in.timeToSail % 2 == 1)
    {
        range1 = { 0, in.timeToSail / 2 + 1 };
        range2 = { in.timeToSail / 2 + 1, in.timeToSail + 1 };
    }
    else
    {
        range1 = { 0, in.timeToSail / 2};
        range2 = { in.timeToSail / 2, in.timeToSail + 1 };
    }

    return lastGreaterRight(range2, in) - findFirstSmallerLeft(range1, in);
}

}  // namespace

std::vector<SingleRace> parse()
{
    // return { {7, 9}, {15, 40}, {30, 200} };
    return { {59, 543}, {68, 1020}, {82, 1664}, {74, 1022} };
}

long long Solution::solve(const std::vector<SingleRace>& input) const
{
    auto res = 1ll;
    for (auto&& s : input)
    {
        res *= resolve(s);
    }
    return res;
}

long long Solution::solve_part2(const std::vector<SingleRace>& input) const
{
    SingleRace sr{ 59688274ll, 543102016641022ll };
    return resolve(sr);
}

}  // namespace year_2023::day6
