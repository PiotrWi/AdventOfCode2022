#pragma once

#include <vector>
#include <fstream>
#include <sstream>

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

using Day4Type = std::pair<std::pair<int, int>, std::pair<int, int>>;
template<>
inline auto toT<Day4Type>(const std::string& in) -> Day4Type
{
    char c;
    Day4Type out;
    std::stringstream lineStream(in);
    lineStream >> out.first.first >> c >> out.first.second >> c >> out.second.first >> c >> out.second.second;
    return out;
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
