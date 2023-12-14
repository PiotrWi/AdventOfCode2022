#include "StringAlgorithms.hpp"

std::string ltrim(const std::string& in)
{
    auto pos = in.find_first_not_of(' ');
    if (pos != std::string::npos)
    {
        return in.substr(in.find_first_not_of(' '));
    }
    return in;
}

std::string rtrim(const std::string& in)
{
    auto count = std::find_if(in.rbegin(), in.rend(), [](char c) { return c != ' '; }) - in.rbegin();
    return in.substr(0, in.size() - count);
}

std::string trim(const std::string& in)
{
    return rtrim(ltrim(in));
}

std::vector<std::string> splitAndTrim(const std::string& in, char c)
{
    std::vector<std::string> out;
    for (decltype(in.size()) i = 0; i < in.size();)
    {
        auto pos = in.find(c, i);
        out.emplace_back(trim(in.substr(i, pos-i)));
        if (pos == std::string::npos)
            break;
        i = pos + 1;
    }
    return out;
}

std::vector<std::string> splitAndTrimByStr(const std::string& in, std::string delimeter)
{
    std::vector<std::string> out;
    for (decltype(in.size()) i = 0; i < in.size();)
    {
        auto pos = in.find(delimeter, i);
        out.emplace_back(trim(in.substr(i, pos-i)));
        if (pos == std::string::npos)
            break;
        i = pos + delimeter.size();
    }
    return out;
}

std::vector<std::string> splitNumbersAndLetters(const std::string& in)
{
    std::vector<std::string> out;
    for (decltype(in.size()) i = 0; i < in.size();)
    {
        std::string number;
        while (std::isdigit(in[i]))
        {
            number.push_back(in[i++]);
        }
        if (number.size())
        {
            out.push_back(number);
        }

        std::string letters;
        while (std::isalpha(in[i]))
        {
            letters.push_back(in[i++]);
        }
        if (letters.size())
        {
            out.push_back(letters);
        }
    }
    return out;
}

std::vector<std::string> splitNumbersAndChars(const std::string& in)
{
    std::vector<std::string> out;
    for (decltype(in.size()) i = 0; i < in.size();)
    {
        std::string number;
        while (i < in.size() && std::isdigit(in[i]))
        {
            number.push_back(in[i++]);
        }
        if (number.size())
        {
            out.push_back(number);
        }

        std::string letters;
        while (i < in.size() && not std::isdigit(in[i]))
        {
            letters.push_back(in[i++]);
        }
        if (letters.size())
        {
            out.push_back(letters);
        }
    }
    return out;
}

std::vector <std::string> transpose(const std::vector <std::string>& input)
{
    std::vector <std::string> transposed(input[0].size());
    for (int row = 0; row < input.size(); ++row)
    {
        for (int col = 0; col < input[0].size(); ++col)
        {
            transposed[col].push_back(input[row][col]);
        }
    }
    return transposed;
}

namespace s_view
{

std::vector < std::span<char> > splitGroups(std::string& in, std::function<bool(char)> groupPredicate)
{
    std::vector<std::span<char>> out;
    for (decltype(in.size()) i = 0; i < in.size();)
    {
        std::string number;
        int j = i;
        while (j < in.size() && groupPredicate(in[j]))
        {
            number.push_back(in[j++]);
        }
        if (number.size())
        {
            out.push_back(std::span(in.data() + i, in.data() + j));
        }

        std::string letters;
        while (j < in.size() && not groupPredicate(in[j]))
        {
            ++j;
        }
        i = j;
    }
    return out;
}

}  // s_view
