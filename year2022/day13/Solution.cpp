#include "Solution.hpp"
#include <algorithm>
#include <cctype>
#include <typeinfo>
#include <parsers/parsers.hpp>

namespace day13
{

const char* fileLoc = "year2022/day13/input.txt";

namespace
{

int parseList(List& node, std::string in);
int parseInteger(Integer& node, std::string in);

int parseInteger(Integer& node, std::string in)
{
    int bytesConsumed;
    for (bytesConsumed = 0; std::isdigit(in[bytesConsumed]); ++bytesConsumed);

    node.val = std::stoi(in.substr(0, bytesConsumed));
    return bytesConsumed;
}

int parseList(List& node, std::string in)
{
    int bytesConsumed;
    for (bytesConsumed = 1; in[bytesConsumed] != ']'; )
    {
        if (in[bytesConsumed] == '[')
        {
            auto listNode = std::make_unique<List>();
            bytesConsumed += parseList(*listNode, in.substr(bytesConsumed));
            node.children.emplace_back(std::move(listNode));
            continue;
        }
        if (in[bytesConsumed] == ',')
        {
            bytesConsumed++;
            continue;
        }
        if (std::isdigit(in[bytesConsumed]))
        {
            auto integerNode = std::make_unique<Integer>();
            bytesConsumed += parseInteger(*integerNode, in.substr(bytesConsumed));
            node.children.emplace_back(std::move(integerNode));
            continue;
        }
    }
    return bytesConsumed + 1;
}

}  // namespace

std::vector<std::unique_ptr<Node>> parse()
{
    std::vector<std::unique_ptr<Node>> out;
    for (auto&& line: parsers::LinesInFileRange(fileLoc))
    {
        if (line == "") {
            continue;
        }
        auto node = std::make_unique<List>();
        parseList(*node, line);
        out.emplace_back(std::move(node));
    }

    return out;
}

namespace
{
bool operator<(const Node& lhs, const Node& rhs);

bool operator<(const Integer& lhs, const Integer& rhs);
bool operator<(const List& lhs, const List& rhs);
bool operator<(const List& lhs, const Integer& rhs);
bool operator<(const Integer& lhs, const List& rhs);

bool operator<(const Integer& lhs, const Integer& rhs)
{
    return lhs.val < rhs.val;
}

bool operator<(const List& lhs, const List& rhs)
{
    for (unsigned i = 0; i < lhs.children.size() && i < rhs.children.size(); ++i)
    {
        if (*lhs.children[i] < *rhs.children[i])
        {
            return true;
        }
        if (*rhs.children[i] < *lhs.children[i])
        {
            return false;
        }
    }
    if (lhs.children.size() < rhs.children.size())
    {
        return true;
    }
    if (rhs.children.size() < lhs.children.size())
    {
        return false;
    }
    return false;
}

bool operator<(const List& lhs, const Integer& rhs)
{
    List rhsList;
    rhsList.children.emplace_back(std::make_unique<Integer>(rhs.val));
    return lhs < rhsList;
}

bool operator<(const Integer& lhs, const List& rhs)
{
    List lhsList;
    lhsList.children.emplace_back(std::make_unique<Integer>(lhs.val));
    return lhsList < rhs;
}

bool operator<(const Node& lhs, const Node& rhs)
{
    if (typeid(lhs) == typeid(Integer) and typeid(rhs) == typeid(Integer))
    {
        return dynamic_cast<const Integer&>(lhs) < dynamic_cast<const Integer&>(rhs);
    }
    if (typeid(lhs) == typeid(List) and typeid(rhs) == typeid(List))
    {
        return dynamic_cast<const List&>(lhs) < dynamic_cast<const List&>(rhs);
    }
    if (typeid(lhs) == typeid(Integer) and typeid(rhs) == typeid(List))
    {
        return dynamic_cast<const Integer&>(lhs) < dynamic_cast<const List&>(rhs);
    }
    if (typeid(lhs) == typeid(List) and typeid(rhs) == typeid(Integer))
    {
        return dynamic_cast<const List&>(lhs) < dynamic_cast<const Integer&>(rhs);
    }
    throw 1;
}

}  // namespace

int Solution::solve(const std::vector<std::unique_ptr<Node>>& in)
{
    int counter = 0;
    for (unsigned int i = 0; i < in.size(); i += 2)
    {
        if (*in[i] < *in[i+1])
        {
            counter += (i / 2) + 1;
        }
    }
    return counter;
}

int Solution::solve_part2(std::vector<std::unique_ptr<Node>>& in)
{
    auto c1 = std::make_unique<List>();
    parseList(*c1, "[[2]]");
    auto c2 = std::make_unique<List>();
    parseList(*c2, "[[6]]");
    in.emplace_back(std::move(c1));
    in.emplace_back(std::move(c2));

    // lol O_O
    auto c11 = std::make_unique<List>();
    parseList(*c11, "[[2]]");
    auto c22 = std::make_unique<List>();
    parseList(*c22, "[[6]]");

    std::sort(in.begin(), in.end(), [](auto&& lhs, auto&& rhs){ return *lhs < *rhs; });

    auto pos1 = 1 + std::find_if(in.begin(), in.end(), [&temp = *c11](auto&& elem) {return (not (temp < *elem)) and (not (*elem < temp)); }) - in.begin();
    auto pos2 = 1 + std::find_if(in.begin(), in.end(), [&temp = *c22](auto&& elem) {return (not (temp < *elem)) and (not (*elem < temp)); }) - in.begin();

    return pos1 * pos2;
}

}  // namepsace day13