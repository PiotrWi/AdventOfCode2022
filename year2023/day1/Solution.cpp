#include "Solution.hpp"

#include <algorithm>
#include <functional>
#include <ranges>
#include <iterator>
#include <utility>
#include <tuple>

#include <utility/RangesUtils.hpp>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day1
{

namespace
{

std::vector<std::pair<const char*, int>> digitLiteralsToValues = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9} };
std::vector<std::pair<const char*, int>> numberLiteralsToValues = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9} };

auto getFirstNumberWord(const std::string& line, std::vector<std::pair<const char*, int>>& numberLiteralsToValues)
{
    int num = 0;
    auto firstNumber = std::string::npos;
    
    for (auto& [str, n] : numberLiteralsToValues)
    {
        auto pos = line.find(str);
        if (pos != std::string::npos && pos < firstNumber)
        {
            firstNumber = pos;
            num = n;
        }
    }
    return num;
}

auto getLastNumberWord(const std::string& line, std::vector<std::pair<const char*, int>>& numberLiteralsToValues)
{
    auto num = 0u;
    auto lastNumber = 0u;

    for (auto& [str, n] : numberLiteralsToValues)
    {
        auto pos = line.rfind(str);
        if (pos != std::string::npos && pos >= lastNumber)
        {
            lastNumber = pos;
            num = n;
        }
    }
    return num;
}

std::tuple<int, int> getFirstAndLastNumber(const std::string& line, std::vector<std::pair<const char*, int>>& numberLiteralsToValues)
{
    auto firstNum = getFirstNumberWord(line, numberLiteralsToValues);
    auto lastNum = getLastNumberWord(line, numberLiteralsToValues);

    return std::make_tuple(firstNum, lastNum);
}

int accumulate(const std::vector<std::string>& input, std::vector<std::pair<const char*, int>>& numberLiteralsToValues)
{
    int sum = 0;
    auto getFirtsAndLastNumberFct = std::bind(getFirstAndLastNumber, std::placeholders::_1, numberLiteralsToValues);
    for (auto&& [first, last] : (input | std::views::transform(getFirtsAndLastNumberFct)))
    {
        sum += first * 10 + last;
    }
    return sum;
}

}  // namespace

std::vector<std::string> parse()
{
    return parsers::getFile(2023, 1).toStringVector();
}

int Solution::solve(const std::vector<std::string>& input) const
{
    return accumulate(input, digitLiteralsToValues);
}


int Solution::solve_part2(const std::vector<std::string>& input) const
{
    return accumulate(input, numberLiteralsToValues);
}

}  // namespace year_2022::day1
