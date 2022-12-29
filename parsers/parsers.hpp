#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <optional>

namespace parsers
{

class LinesInFileRange
{
public:
    explicit LinesInFileRange(const char* fileLocation);
    ~LinesInFileRange();
    bool hasData() const;

private:
    struct TIterator
    {
        TIterator(LinesInFileRange& parent);
        std::string operator*();
        void operator++();
        bool hasData() const;
    private:
        LinesInFileRange& parent_;
    };

    struct TEndTag{};

public:
    TIterator begin();
    TEndTag end();

private:
    std::optional<std::string> line_;
    std::fstream inputFile_;

public:
    friend bool operator==(const LinesInFileRange::TIterator& lhs, LinesInFileRange::TEndTag);
    friend bool operator!=(const LinesInFileRange::TIterator& lhs, LinesInFileRange::TEndTag);
};

template <typename T>
auto toT(const std::string&) -> T;

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
