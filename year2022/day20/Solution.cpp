#include "Solution.hpp"
#include <parsers/parsers.hpp>
#include <iostream>

namespace parsers
{

template<>
inline auto toT<int>(const std::string &in) -> int
{
    return std::stoi(in);
}

}

namespace day20
{

std::vector<int> parse()
{
    const char* fileLoc = "year2022/day20/input.txt";
    return parsers::parse<int>(fileLoc);
}

namespace
{

struct Node
{
    Node* prev;
    Node* next;
    long long val;
};

Node* getNth(std::vector<Node>& nodes, long n)
{
    Node* node = &nodes[0];
    while (node->val != 0)
    {
        node = node->next;
    }

    for (int i = 0; i < n; ++i)
    {
        node = node->next;
    }
    return node;
}

}

int Solution::solve(std::vector<int> in)
{
    std::vector<Node> nodes(in.size(), Node{});
    for (unsigned i = 0; i < in.size(); ++i)
    {
        nodes[i].next = &(nodes[(i+1) % in.size()]);
        nodes[i].prev = &(nodes[(i + in.size() -1) % in.size()]);
        nodes[i].val = in[i];
    }

    for (unsigned i = 0; i < in.size(); ++i)
    {
        auto shift = nodes[i].val;
        while (shift < 0)
        {
            shift += (in.size() - 1);
        }
        while (shift >= (int)in.size() -1)
        {
            shift -= (in.size() - 1);
        }
        if (shift == 0)
        {
            continue;
        }

        // mergeGap
        auto& node = nodes[i];
        node.prev->next = node.next;
        node.next->prev = node.prev;

        Node* nextDescendant = node.next;
        for (shift = shift-1; shift != 0; shift--)
        {
            nextDescendant = nextDescendant->next;
        }

        // push
        node.next = nextDescendant->next;
        node.prev = nextDescendant;
        nextDescendant->next = &node;
        node.next->prev = &node;

    }

    return getNth(nodes, 1000)->val + getNth(nodes, 2000)->val + getNth(nodes, 3000)->val;
}

long long Solution::solve_part2(std::vector<int> in)
{
    std::vector<Node> nodes(in.size(), Node{});
    for (unsigned i = 0; i < in.size(); ++i)
    {
        nodes[i].next = &(nodes[(i+1) % in.size()]);
        nodes[i].prev = &(nodes[(i + in.size() -1) % in.size()]);
        nodes[i].val = in[i] * 811589153ll;
    }


    for (auto tenTimes = 0; tenTimes < 10; tenTimes++)
    {
        for (unsigned i = 0; i < in.size(); ++i)
        {
            auto shift = nodes[i].val;
            if (shift < 0)
            {
                auto n = shift / (long)(in.size() - 1);
                shift += -1 * n * (in.size() - 1);
                while (shift < 0)
                {
                    shift += (in.size() - 1);
                }
            }
            if (shift >= (int)in.size() -1)
            {
                auto n = shift / (long)(in.size() - 1);
                shift -= 1 * n * (in.size() - 1);
                while (shift >= (int)in.size() -1)
                {
                    shift -= (in.size() - 1);
                }
            }

            if (shift == 0)
            {
                continue;
            }

            // mergeGap
            auto& node = nodes[i];
            node.prev->next = node.next;
            node.next->prev = node.prev;

            Node* nextDescendant = node.next;
            for (shift = shift-1; shift != 0; shift--)
            {
                nextDescendant = nextDescendant->next;
            }

            // push
            node.next = nextDescendant->next;
            node.prev = nextDescendant;
            nextDescendant->next = &node;
            node.next->prev = &node;
        }
    }

    return getNth(nodes, 1000)->val + getNth(nodes, 2000)->val + getNth(nodes, 3000)->val;
}

}