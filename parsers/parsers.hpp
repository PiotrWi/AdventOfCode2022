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

template<>
inline auto toT<int>(const std::string& in) -> int
{
    return std::stoi(in);
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

inline std::vector<std::vector<int>> parseMatrixOfChars(const char* fileLoc)
{
    std::vector<std::vector<int>> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    while (inputFile)
    {
        std::getline(inputFile, line);
        if (line == "")
        {
            continue;
        }
        std::vector<int> singleRow;
        std::stringstream ss(line);
        char c;
        while (ss.get(c))
        {
            singleRow.push_back(c);
        }
        out.emplace_back(std::move(singleRow));
    }

    inputFile.close();
    return out;
}

}  // namespace parsers
