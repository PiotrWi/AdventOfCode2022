#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <iterator>
#include <utility>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day1
{

const char* fileLoc = "year2023/day1/input.txt";

namespace
{

std::vector<int> filter_digits(const std::string& line)
{
    std::vector<int> out = {};
    for (int num : line | std::views::filter(std::isdigit) | std::views::transform([](auto&& i) { return i - '0'; }))
    {
        out.push_back(num);
    }

    return out;
}

auto getFirstDigit(const std::string& line)
{
    int num = 0;
    auto iter = std::find_if(line.begin(), line.end(), std::isdigit);
    if (iter != line.end())
    {
        num = *iter - '0';
    }
    return std::make_tuple(iter - line.begin(), num);
}


auto getLastDigit(const std::string& line)
{
    int num = 0;
    auto iter = std::find_if(line.rbegin(), line.rend(), std::isdigit);
    if (iter != line.rend())
    {
        num = *iter - '0';
    }
    return std::make_tuple(line.rend() - iter - 1, num);
}

auto getFirstNumberWord(const std::string& line)
{
    int num = 0;
    auto firstNumber = std::string::npos;
    
    static std::vector<std::pair<const char*, int>> vec = { {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9} };
    for (auto& [str, n] : vec)
    {
        auto pos = line.find(str);
        if (pos != std::string::npos && pos < firstNumber)
        {
            firstNumber = pos;
            num = n;
        }
    }
    return std::make_tuple(firstNumber, num);
}

auto getLastNumberWord(const std::string& line)
{
    int num = 0;
    int lastNumber = 0;

    static std::vector<std::pair<const char*, int>> vec = { {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9} };
    for (auto& [str, n] : vec)
    {
        auto pos = line.rfind(str);
        if (pos != std::string::npos && pos > lastNumber)
        {
            lastNumber = pos;
            num = n;
        }
    }
    return std::make_tuple(lastNumber, num);
}

std::tuple<int, int> getFirstAndLastDigit(const std::string& line)
{
    auto [firtsDigitPos, firstDigit] = getFirstDigit(line);
    auto [lastDigitPos, lastDigit] = getLastDigit(line);

    return std::make_tuple(firstDigit, lastDigit);
}

std::tuple<int, int> get_first_and_last_number(const std::string& line)
{
    std::vector<int> out = {};

    int firstNum, lastNum;

    auto [firtsDigitPos, firstDigit] = getFirstDigit(line);
    auto [firtsWordPos, firstWordDigit] = getFirstNumberWord(line);
    if (firtsDigitPos < firtsWordPos)
    {
        firstNum = firstDigit;
    }
    else
    {
        firstNum = firstWordDigit;
    }

    auto [lastDigitPos, lastDigit] = getLastDigit(line);
    auto [lastWordPos, lastWordDigit] = getLastNumberWord(line);
    if (lastWordPos > lastDigitPos)
    {
        lastNum = lastWordDigit;
    }
    else
    {
        lastNum = lastDigit;
    }

    return std::make_tuple(firstNum, lastNum);
}

}  // namespace

std::vector<std::string> parse()
{
    std::vector<std::string> out;
    for (auto&& line : parsers::LinesInFileRange(fileLoc))
    {
        out.emplace_back(std::move(line));
    }

    return out;
}

int Solution::solve(const std::vector<std::string>& input) const
{
    int sum = 0;
    for (auto&& [first, last] : input | std::views::transform(getFirstAndLastDigit))
    {
        sum += first * 10 + last;
    }
    return sum;
}


int Solution::solve_part2(const std::vector<std::string>& input) const
{
    int sum = 0;
    for (auto&& [first, last] : input | std::views::transform(get_first_and_last_number))
    {
        sum += first * 10 + last;
    }
    return sum;
}

}  // namespace year_2022::day1