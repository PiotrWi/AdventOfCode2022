#pragma once

#include <vector>
#include <fstream>

namespace parsers
{

template <typename T>
auto toT(const std::string&) -> T;

template<>
inline auto toT<std::string>(const std::string& in) -> std::string
{
    return in;
}

template<>
inline auto toT<std::pair<char, char>>(const std::string& in) -> std::pair<char, char>
{
    return std::pair<char, char>{in[0], in[2]};
}

template <typename T>
auto parse(const char* fileLocation) -> std::vector<T>
{
    std::vector<T> out;
    std::fstream inputFile(fileLocation);
    std::string line;

    while (std::getline(inputFile, line))
    {
        out.emplace_back(toT<T>(line));
    }

    inputFile.close();
    return out;
}

}  // namespace parsers
