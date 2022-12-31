#include "Solution.hpp"

#include <parsers/parsers.hpp>
#include <iostream>

namespace day25
{

const char *fileLoc = "day25/input.txt";

std::vector<std::string> parse()
{
    return parsers::parse<std::string>(fileLoc);
}

long snafuToInt(const std::string& s)
{
    long pow = 1;
    long out = 0;
    for (int i = int(s.size())-1; i>=0; --i)
    {
        if (s[i] == '0' || s[i] == '1' || s[i] == '2')
        {
            out += pow * (s[i] - '0');
        }
        if (s[i] == '-')
        {
            out -= pow;
        }
        if (s[i] == '=')
        {
            out -= 2 * pow;
        }
        pow *= 5;
    }
    return out;
}

std::string intToSnafu(long num)
{
    std::string out;

    while (num != 0)
    {
        auto rest = num % 5;
        int carry = 0;
        if (rest == 0 || rest == 1 || rest == 2)
        {
            out.push_back('0' + rest);
            carry = 0;
        }
        if (rest == 3)
        {
            carry = 1;
            out.push_back('=');
        }
        if (rest == 4)
        {
            carry = 1;
            out.push_back('-');
        }
        num = num / 5 + carry;
    }
    std::reverse(out.begin(), out.end());
    return out;
}


std::string Solution::solve(std::vector<std::string> input)
{
    long sumInDec = 0;
    for (auto&& snafu : input)
    {
        long val = snafuToInt(snafu);
        sumInDec += val;
    }
    std::cout << sumInDec << std::endl;
    std::cout << intToSnafu(sumInDec) << std::endl;
    std::cout << snafuToInt(intToSnafu(sumInDec))  << std::endl;
    return intToSnafu(sumInDec);
}
}