#include "Solution.hpp"
#include <string>
#include <fstream>

namespace day6
{
const char* fileLoc = "day6/input.txt";

std::string parse()
{
    std::fstream inputFile(fileLoc);
    std::string line;
    std::getline(inputFile, line);
    inputFile.close();

    return line;
}

namespace
{

int findPosition(std::string input, int n)
{
    int oneOccurences = 0;
    int letterCount[30] = {};
    for (int i = 0; i < n; ++i)
    {
        auto letter = input[i];
        letterCount[letter-'a']++;

        if (letterCount[letter-'a'] == 1)
        {
            ++oneOccurences;
        }
        if (letterCount[letter-'a'] == 2)
        {
            --oneOccurences;
        }
    }
    if (oneOccurences == n)
    {
        return oneOccurences;
    }

    for (unsigned int i = 1; i < input.size() - n + 2; ++i)
    {
        auto letterOut = input[i-1];
        letterCount[letterOut-'a']--;
        if (letterCount[letterOut-'a'] == 1)
        {
            ++oneOccurences;
        }
        if (letterCount[letterOut-'a'] == 0)
        {
            --oneOccurences;
        }

        auto letterIn = input[i+n-1];
        letterCount[letterIn-'a']++;
        if (letterCount[letterIn-'a'] == 1)
        {
            ++oneOccurences;
        }
        if (letterCount[letterIn-'a'] == 2)
        {
            --oneOccurences;
        }

        if (oneOccurences == n)
        {
            return i+oneOccurences;
        }
    }

    return -1;
}
}

int Solution::solve(std::string input) const
{
    return findPosition(input, 4);
}

int Solution::solve_part2(std::string input) const
{
    return findPosition(input, 14);
}

}  // namespace day56
