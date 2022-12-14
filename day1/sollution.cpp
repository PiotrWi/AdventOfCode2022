#include "sollution.hpp"
#include <algorithm>
#include <numeric>
#include <fstream>

namespace day1
{

const char* fileLoc = "day1/input.txt";

std::vector<std::vector<int>> parse()
{
    std::vector<std::vector<int>> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    std::vector<int> singleInput;
    while (inputFile)
    {
        std::getline(inputFile, line);
        if (not line.empty())
        {
            singleInput.emplace_back(std::stoi(line));
        }
        else
        {
            out.emplace_back(std::move(singleInput));
            singleInput = {};
        }
    }

    inputFile.close();
    return out;
}

int Sollution::getSollution(const std::vector<std::vector<int>>& in)
{
    auto max = 0;
    for (unsigned int i = 0; i < in.size(); ++i)
    {
        auto current = std::accumulate(in[i].begin(), in[i].end(), 0);
        if (current > max)
            max = current;
    }
    return max;
}

// Not the optimal one, but short to code.
int Sollution::getSollution_part2(const std::vector<std::vector<int>>& in)
{
    std::vector<int> out(in.size(), 0);

    for (unsigned int i = 0; i < in.size(); ++i)
    {
        out[i] = std::accumulate(in[i].begin(), in[i].end(), 0);
    }
    std::sort(out.begin(), out.end());

    return *out.rbegin() + *(out.rbegin() + 1) + *(out.rbegin()+2);
}

}
