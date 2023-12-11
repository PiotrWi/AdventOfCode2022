#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <list>
#include <deque>
#include <unordered_map>
#include <sstream>
#include <utility>

#include <StringAlgorithms/StringAlgorithms.hpp>
#include <parsers/parsers.hpp>

namespace day16
{

namespace
{

Node parseLine(const std::string &s)
{
    Node node;
    std::stringstream ss(s);
    std::string ingore;
    ss >> ingore >> node.label;
    ss >> ingore >> ingore;

    std::string rateStrToSplit;
    ss >> rateStrToSplit;
    node.flowRate = std::stoi(splitAndTrim(rateStrToSplit.substr(0, rateStrToSplit.size() - 1), '=')[1]);
    ss >> ingore >> ingore >> ingore >> ingore;

    std::string neighboursToSplit;
    std::getline(ss, neighboursToSplit);
    for (auto &&neighbour: splitAndTrim(neighboursToSplit, ',')) {
        node.neighbours.push_back(neighbour);
    }
    return node;
}

}

std::map<std::string, Node> parse()
{
    std::map<std::string, Node> out;

    for (auto&& line : parsers::getFile(2022, 16))
    {
        auto node = parseLine(line);
        out[node.label] = node;
    }

    return out;
}

namespace
{

struct NodeInt
{
    unsigned char id;
    unsigned char flowRate;
    std::vector<unsigned char> neighbours;
};

auto mapInputLabelsToInts(const std::map<std::string, Node> in)
{
    std::map<std::string, int> strToInt;
    std::vector<NodeInt> out;
    auto index = 0;
    for (auto it = in.begin(); it != in.end(); ++it, ++index) {
        strToInt[it->first] = index;
    }

    for (auto it = in.begin(); it != in.end(); ++it, ++index) {
        NodeInt node;
        node.id = strToInt[it->first];
        node.flowRate = it->second.flowRate;
        for (auto &&neigbour: it->second.neighbours) {
            node.neighbours.emplace_back(strToInt[neigbour]);
        }
        out.emplace_back(node);
    }

    return out;
}

auto fillCostForSingleNode(const std::vector<NodeInt> nodes,
                           std::vector<std::vector<int> > &costs,
                           const NodeInt &node)
{
    unsigned filled = 0;
    std::list<std::pair<int/*cost*/, int /*nodeId*/> > nodesToVisit;
    nodesToVisit.push_back({0, node.id});
    for (auto it = nodesToVisit.begin(); filled != nodes.size(); it++) {
        auto &destinationNode = nodes[it->second];
        if (costs[node.id][destinationNode.id] != -1) {
            continue;
        }
        auto cost = it->first;
        costs[node.id][destinationNode.id] = cost;
        ++filled;

        for (auto &&neigbourId: destinationNode.neighbours) {
            if (costs[node.id][neigbourId] == -1)
                nodesToVisit.emplace_back(cost + 1, neigbourId);
        }
    }
}

auto buildCostMatrix(std::vector<NodeInt> nodes)
{
    std::vector<std::vector<int> > out(nodes.size(), std::vector<int>(nodes.size(), -1));
    for (const auto &node: nodes) {
        fillCostForSingleNode(nodes, out, node);
    }
    return out;
}

long currentMax = 0;
using TNodesToVisitList =  std::deque<int>;

std::pair<unsigned int, int> maxValAndMinCost(const std::vector<std::vector<int> >& costMartix, const std::vector<NodeInt>& nodes, int node, const TNodesToVisitList& nodesWorthToVisit)
{
    unsigned char max = 0;
    int minCost = 30;

    for (auto it = nodesWorthToVisit.begin(); it != nodesWorthToVisit.end(); it++)
    {
        minCost = std::min(minCost, costMartix[node][*it]);
        max = std::max(nodes[*it].flowRate, max);
    }
    return std::make_pair(max, minCost);
}

long heuristic2(const std::vector<std::vector<int> >& costMartix, int timeLeft, int node, const std::vector<NodeInt>& nodes, const TNodesToVisitList& nodesWorthToVisit)
{
    auto mm = maxValAndMinCost(costMartix, nodes, node,  nodesWorthToVisit);

    auto timeleftAfterReachClosest = timeLeft - mm.second -1;
    int out = 0;
    for (int time = timeleftAfterReachClosest; time > 0; time = time - 2)
    {
        out += time * mm.first;
    }
    return out;
}

TNodesToVisitList filterOnlyWrothToVisit(const std::vector<NodeInt>& nodes)
{
    TNodesToVisitList nodesWorhtToVisit;
    for (auto&& worthToVisit : nodes
        | std::views::filter([](auto&& elem) { return elem.flowRate > 0; })
        | std::views::transform([](auto&& elem) { return elem.id; }))
    {
        nodesWorhtToVisit.push_back(worthToVisit);
    }
    return nodesWorhtToVisit;
}

}  // namespace

namespace classic
{

const int Timeout = 30;

long findMaxFromAA(const std::vector<std::vector<int> > &costMartix,
                   const std::vector<NodeInt>& nodes,
                   int node,
                   TNodesToVisitList& nodesWorthToVisit,
                   long currentFloat,
                   long cuttentValue,
                   long elapsedTime)
{
    auto timeLeft = Timeout - elapsedTime;
    long max = cuttentValue + currentFloat * timeLeft;
    if (max + heuristic2(costMartix, timeLeft, node, nodes, nodesWorthToVisit) < currentMax)
    {
        return 0;
    }
    for (unsigned int i = 0; i < nodesWorthToVisit.size(); ++i) {
        auto candidate = nodesWorthToVisit.front();
        nodesWorthToVisit.pop_front();

        auto costToOpenPipeInCanditate = 1 + costMartix[node][candidate];
        if (costToOpenPipeInCanditate + elapsedTime < Timeout) {
            max = std::max(max, findMaxFromAA(costMartix,
                                              nodes,
                                              candidate,
                                              nodesWorthToVisit,
                                              currentFloat + nodes[candidate].flowRate,
                                              cuttentValue + currentFloat * costToOpenPipeInCanditate,
                                              elapsedTime + costToOpenPipeInCanditate));
        }

        nodesWorthToVisit.push_back(candidate);
    }
    return max;
}

}

namespace elephant
{

const int Timeout = 26;

long releaseElephant(const std::vector<std::vector<int> >& costMartix,
    const std::vector<NodeInt>& nodes,
    int node,
    TNodesToVisitList& nodesWorthToVisit,
    long currentFloat,
    long cuttentValue,
    long elapsedTime)
{
    auto timeLeft = Timeout - elapsedTime;
    long max = cuttentValue + currentFloat * timeLeft;
    if (max + heuristic2(costMartix, timeLeft, node, nodes, nodesWorthToVisit) < currentMax)
    {
        return 0;
    }
    for (unsigned int i = 0; i < nodesWorthToVisit.size(); ++i) {
        auto candidate = nodesWorthToVisit.front();
        nodesWorthToVisit.pop_front();

        auto costToOpenPipeInCanditate = 1 + costMartix[node][candidate];
        if (costToOpenPipeInCanditate + elapsedTime < Timeout) {
            max = std::max(max, releaseElephant(costMartix,
                nodes,
                candidate,
                nodesWorthToVisit,
                currentFloat + nodes[candidate].flowRate,
                cuttentValue + currentFloat * costToOpenPipeInCanditate,
                elapsedTime + costToOpenPipeInCanditate));
        }

        nodesWorthToVisit.push_back(candidate);
    }
    currentMax = std::max(currentMax, max);
    return max;
}

uint64_t allNodesWorthToVisitMask = 0ull;
uint64_t getAllNodesWorthToVisitMask(const TNodesToVisitList& nodesWorhtToVisit)
{
    uint64_t mask = 0ull;
    for (auto&& index : nodesWorhtToVisit)
    {
        mask |= (1ull << index);
    }
    return mask;
}

std::unordered_map< uint64_t, int> maxMaluesWithNodesStillToVisit;

void fillMaxFromAA(const std::vector<std::vector<int> >& costMartix,
    const std::vector<NodeInt>& nodes,
    int node,
    TNodesToVisitList& nodesWorthToVisit,
    long currentFloat,
    long cuttentValue,
    long elapsedTime)
{
    auto timeLeft = Timeout - elapsedTime;
    long currentNodeValueIfNoMoreOpen = cuttentValue + currentFloat * timeLeft;
    auto& cachedVal = maxMaluesWithNodesStillToVisit[getAllNodesWorthToVisitMask(nodesWorthToVisit)];
    if (cachedVal < currentNodeValueIfNoMoreOpen)
    {
        cachedVal = currentNodeValueIfNoMoreOpen;
    }

    for (unsigned int i = 0; i < nodesWorthToVisit.size(); ++i)
    {
        auto candidate = nodesWorthToVisit.front();
        nodesWorthToVisit.pop_front();

        auto costToOpenPipeInCanditate = 1 + costMartix[node][candidate];
        if (costToOpenPipeInCanditate + elapsedTime < Timeout) {
            fillMaxFromAA(costMartix,
                nodes,
                candidate,
                nodesWorthToVisit,
                currentFloat + nodes[candidate].flowRate,
                cuttentValue + currentFloat * costToOpenPipeInCanditate,
                elapsedTime + costToOpenPipeInCanditate);
        }

        nodesWorthToVisit.push_back(candidate);
    }
}

int calulateElemphantForDifferentNodes(
    const std::vector<std::vector<int> >& costMartix,
    const std::vector<NodeInt>& nodes)
{
    std::vector<std::pair<uint64_t, int>> sortedVector;
    for (auto&& nodesToVal : maxMaluesWithNodesStillToVisit)
    {
        sortedVector.push_back(nodesToVal);
    }
    std::sort(sortedVector.rbegin(), sortedVector.rend(), [](const auto& lhs, const auto& rhs)
        {
            return lhs.second < rhs.second;
        });

    for (auto&& nodesToVal : sortedVector)
    {
        auto nodesMask = nodesToVal.first;
        TNodesToVisitList nodesWorthToVisit;
        for (; nodesMask; nodesMask = _blsr_u64(nodesMask))
        {
            unsigned char nodeIndex = _tzcnt_u64(nodesMask);
            nodesWorthToVisit.push_back(nodeIndex);
        }
        releaseElephant(costMartix, nodes, 0, nodesWorthToVisit, 0, nodesToVal.second, 0);
    }

    return currentMax;
}

}

int Solution::solve(std::map<std::string, Node> in)
{
    currentMax = 0;

    auto nodes = mapInputLabelsToInts(in);
    std::vector<std::vector<int> > costMartix = buildCostMatrix(nodes);
    auto nodesWorhtToVisit = filterOnlyWrothToVisit(nodes);

    auto max = classic::findMaxFromAA(costMartix, nodes, 0, nodesWorhtToVisit, 0, 0, 0);

    return max;
}

int Solution::solve_part2(std::map<std::string, Node> in)
{
    currentMax = 0;
    auto nodes = mapInputLabelsToInts(in);
    std::vector<std::vector<int> > costMartix = buildCostMatrix(nodes);
    auto nodesWorhtToVisit = filterOnlyWrothToVisit(nodes);
    elephant::allNodesWorthToVisitMask = elephant::getAllNodesWorthToVisitMask(nodesWorhtToVisit);

    elephant::fillMaxFromAA(costMartix, nodes, 0, nodesWorhtToVisit, 0, 0, 0);

    return elephant::calulateElemphantForDifferentNodes(costMartix, nodes);
}

}  // namespace day16
