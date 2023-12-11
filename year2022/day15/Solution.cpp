#include "Solution.hpp"
#include <set>
#include <sstream>
#include <parsers/parsers.hpp>

namespace day15
{

namespace
{

std::pair<SensorLocation, BeaconLocation> parseLine(const std::string& s)
{
    std::pair<SensorLocation, BeaconLocation> out;

    std::stringstream ss(s);
    std::string ingore;
    char c;
    ss >> ingore >> ingore >> c >> c;
    ss >> out.first.x >> c >> c >> c >> out.first.y >> c;
    ss >> ingore >> ingore >> ingore >> ingore;
    ss >> c >> c;
    ss >> out.second.x  >> c >> c >> c >>out.second.y;

    return out;
}

}

std::vector<std::pair<SensorLocation, BeaconLocation> > parse()
{
    std::vector<std::pair<SensorLocation, BeaconLocation> > out;
    for (auto&& line : parsers::getFile(2022, 15))
    {
        out.push_back(parseLine(line));
    }
    return out;
}

namespace
{

struct KeyCompare
{
    bool operator()(std::pair<int, int> lhs, std::pair<int, int> rhs) const
    {
        return lhs.first < rhs.first;
    }
};
using TRangesSet = std::set<std::pair<int, int>, KeyCompare>;

void addToSetAndIntegrate(TRangesSet& ranges, std::pair<int, int> range)
{
    std::vector<std::set<std::pair<int, int>, KeyCompare>::iterator> elementsMerged;

    // tryToMergeWithLower
    auto upperBound = ranges.upper_bound(range);
    if (upperBound != ranges.begin())
    {
        auto lowerElem = upperBound;
        lowerElem--;
        if (lowerElem->second >= range.first)
        {
            elementsMerged.push_back(lowerElem);
            range.first = lowerElem->first;
            range.second = std::max(lowerElem->second, range.second);
        }
    }
    for (auto it = upperBound; it != ranges.end() && it->first <= range.second; ++it)
    {
        elementsMerged.push_back(it);
        range.second = std::max(it->second, range.second);
    }

    for (auto it : elementsMerged)
    {
        ranges.erase(it);
    }
    ranges.insert(range);
}

}

namespace
{

TRangesSet fillRanges(const std::vector<std::pair<SensorLocation, BeaconLocation> >& in, int line)
{
    TRangesSet ranges;

    for (auto&& singleReadout : in)
    {
        auto& sensor = singleReadout.first;
        auto& beacon = singleReadout.second;
        auto sensorBeaconDistance = manhatanAbs(sensor - beacon);
        auto distanceWithoutAnyBeacon = sensorBeaconDistance;
        auto yDist = abs(sensor.y-line);
        if (distanceWithoutAnyBeacon > yDist)
        {
            auto middleOnLine = sensor.x;
            std::pair<int, int> range;
            range.first = middleOnLine - (distanceWithoutAnyBeacon - yDist);
            range.second = middleOnLine + (distanceWithoutAnyBeacon - yDist) + 1;

            addToSetAndIntegrate(ranges, range);
        }
    }
    return ranges;
}

TRangesSet fillRanges2(std::vector<std::pair<SensorLocation, BeaconLocation> >& in, int line)
{
    TRangesSet ranges;

    for (auto&& singleReadout : in)
    {
        auto& sensor = singleReadout.first;
        auto& beacon = singleReadout.second;
        auto sensorBeaconDistance = manhatanAbs(sensor - beacon);
        auto distanceWithoutAnyBeacon = sensorBeaconDistance;
        auto yDist = abs(sensor.y-line);
        if (distanceWithoutAnyBeacon > yDist)
        {
            auto middleOnLine = sensor.x;
            std::pair<int, int> range;
            range.first = middleOnLine - (distanceWithoutAnyBeacon - yDist);
            range.second = middleOnLine + (distanceWithoutAnyBeacon - yDist) + 1;

            if (range.first<0) range.first = 0;
            if (range.second > 4000000) range.second = 4000000;
            addToSetAndIntegrate(ranges, range);
        }
    }
    return ranges;
}

}

int Solution::solve(std::vector<std::pair<SensorLocation, BeaconLocation> > in)
{
    auto line = 2000000;
    auto ranges = fillRanges(in, line);

    int counter = 0;
    for (auto&& elem : ranges)
    {
        counter += elem.second - elem.first;
    }

    std::set<int> beaconsOn10;
    for (auto&& singleReadout : in)
    {
        auto& beacon = singleReadout.second;
        if (beacon.y == line)
        {
            beaconsOn10.insert(beacon.x);
        }
    }

    for (auto&& beacon : beaconsOn10)
    {
        for (auto&& elem : ranges)
        {
            if (beacon >= elem.first && beacon < elem.second)
            {
                counter--;
            }
        }
    }
    return counter;

}

unsigned long long Solution::solve_part2(std::vector<std::pair<SensorLocation, BeaconLocation> > in)
{
    for (int line = 0; line <= 4000000; ++line)
    {
        auto ranges = fillRanges2(in, line);
        if (ranges.size() == 2)
        {
            unsigned long long out = ranges.begin()->second;
            out = out * 4000000ull + (unsigned long long)line;

            return out;
        }
    }
    return -1;
}

}
