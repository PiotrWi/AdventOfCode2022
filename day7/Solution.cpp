#include "Solution.hpp"

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <parsers/parsers.hpp>


namespace day7
{
const char* fileLoc = "day7/input.txt";

std::vector<Command> parse()
{
    std::vector<Command> out;

    for (auto&& line : parsers::LinesInFileRange(fileLoc))
    {
        if (line[0] == '$')
        {
            out.push_back(Command{line, std::vector<std::string>{}});
        }
        else
        {
            out.back().output.emplace_back(line);
        }
    }
    return out;
}

struct Node
{
    std::string name = "";
    bool isDir = false;
    uint size = 0;
    uint totalSize = 0;
    std::vector<Node*> children = {};
};

namespace
{

std::string createLoc (std::string parrent, std::string name)
{
    if (parrent.back() == '/')
    {
        parrent += name;
    }
    else
    {
        parrent += "/" + name;
    }
    return parrent;
}

std::map<std::string, Node> buildTree(const std::vector<Command>& input)
{
    std::map<std::string, Node> tree;
    std::string currentNodeLoc = "/";
    tree["/"] = Node{};
    tree["/"].isDir = true;

    for (auto&& command : input)
    {
        if (command.command_.substr(2, 2) == "cd")
        {
            auto argument = command.command_.substr(5);
            if (argument[0] == '/')
            {
                currentNodeLoc = argument;
                continue;
            }
            if (argument == "..")
            {
                currentNodeLoc = currentNodeLoc.substr(0, currentNodeLoc.find_last_of("/"));
                continue;
            }
            else
            {
                currentNodeLoc = createLoc(currentNodeLoc, argument);
                continue;
            }
        }
        if (command.command_.substr(2, 2) == "ls")
        {
            auto& node = tree[currentNodeLoc];
            for (auto&& line : command.output)
            {
                if (line.substr(0,3) == "dir")
                {
                    Node n;
                    n.name = line.substr(4);
                    n.isDir = true;
                    tree[createLoc(currentNodeLoc, n.name)] = n;
                    node.children.push_back(&tree[createLoc(currentNodeLoc, n.name)]);
                }
                else
                {
                    Node n;
                    std::stringstream str(line);
                    str >> n.size >> n.name;
                    n.isDir = false;
                    tree[createLoc(currentNodeLoc, n.name)] = n;
                    node.children.push_back(&tree[createLoc(currentNodeLoc, n.name)]);
                }
            }
        }
    }
    return tree;
}

int fillSize(Node* node)
{
    if (node->isDir == false)
    {
        return node->size;
    }

    for (auto* child: node->children)
    {
        node->totalSize += fillSize(child);
    }
    return node->totalSize;
}

int aggregateSize(Node* node)
{
    if (node->isDir == false)
    {
        return 0;
    }
    auto thisSize = (node->totalSize <= 100000) ? node->totalSize : 0;
    auto out = 0;
    for (auto* child: node->children)
    {
        out += aggregateSize(child);
    }
    return out + thisSize;
}

}

int Solution::solve(const std::vector<Command>& input)
{
    std::map<std::string, Node> tree = buildTree(input);
    fillSize(&tree["/"]);
    return aggregateSize(&tree["/"]);

}
int Solution::solve_part2(const std::vector<Command>& input)
{
    std::map<std::string, Node> tree = buildTree(input);
    fillSize(&tree["/"]);
    auto usedSpace = tree["/"].totalSize;
    auto spaceToFree = 30000000 + usedSpace - 70000000;

    auto min = 70000000u;
    for (const auto&[key, node] : tree)
    {
        if (node.totalSize >= spaceToFree and node.totalSize < min)
        {
            min = node.totalSize;
        }
    }
    return min;

}

}  // namespace day7
