#include "Solution.hpp"

#include <numeric>
#include <ranges>
#include <tuple>
#include <set>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace
{

struct Number
{
    bool isPartial = false;
    int number;
};

bool isInBoundsAndFilled(int row, int col, const std::vector <std::vector <int> >& posToNumber)
{
    return row >= 0 && row < posToNumber.size() &&
        col >= 0 && col < posToNumber[0].size() &&
        posToNumber[row][col] != -1;
}

void markIsPartial(int row, int col, const std::vector <std::vector <int> >& posToNumber, std::vector<Number>& numbers)
{
    if (not isInBoundsAndFilled(row, col, posToNumber)) return;

    auto index = posToNumber[row][col];
    numbers[index].isPartial = true;
}

void fillGearPart(int row, int col, const std::vector <std::vector <int> > posToNumber, const std::vector<Number>& numbers, std::set<int>& gearsIndexes)
{
    if (not isInBoundsAndFilled(row, col, posToNumber)) return;

    auto index = posToNumber[row][col];
    gearsIndexes.insert(index);
}

auto getNumbersAndOcupancyTable(const std::vector<std::string>& input)
{
    std::vector <Number> numbers;
    std::vector <std::vector <int> > numbersOccupancy(input.size(), std::vector<int>(input[0].size(), -1));

    for (auto row = 0u; row < input.size(); ++row)
    {
        auto words = splitNumbersAndChars(input[row]);
        auto col = 0u;
        for (auto word : words)
        {
            if (std::isdigit(word[0]))
            {
                numbers.emplace_back(false, std::stoi(word));
                for (auto i = 0u; i < word.size(); ++i)
                {
                    numbersOccupancy[row][col + i] = numbers.size() - 1;
                }
            }
            col += word.size();
        }
    }

    return std::make_tuple(numbers, numbersOccupancy);
}

}  // namespace

namespace year_2023::day3
{

 const char* fileLoc = "year2023/day3/input.txt";

std::vector<std::string> parse()
{
    return parsers::LinesInFileRange(fileLoc).toStringVector();
}

long long Solution::solve(const std::vector<std::string>& input) const
{
    auto [numbers, numbersOccupancy] = getNumbersAndOcupancyTable(input);

    for (int row = 0; row < input.size(); ++row)
    {
        for (int col = 0; col < input[0].size(); ++col)
        {
            if (not std::isdigit(input[row][col]) && input[row][col] != '.')
            {
                markIsPartial(row - 1, col-1, numbersOccupancy, numbers);
                markIsPartial(row - 1, col, numbersOccupancy, numbers);
                markIsPartial(row - 1, col +1, numbersOccupancy, numbers);
                markIsPartial(row + 1, col - 1, numbersOccupancy, numbers);
                markIsPartial(row + 1, col, numbersOccupancy, numbers);
                markIsPartial(row + 1, col + 1, numbersOccupancy, numbers);
                markIsPartial(row, col - 1, numbersOccupancy, numbers);
                markIsPartial(row, col + 1, numbersOccupancy, numbers);
            }
        }
    }

    auto parial = numbers
        | std::views::filter([](auto&& e) { return e.isPartial; })
        | std::views::transform([](auto&& e) { return e.number;  });
    return std::accumulate(parial.begin(), parial.end(), 0ull);
}


long long Solution::solve_part2(const std::vector<std::string>& input) const
{
    auto [numbers, numbersOccupancy] = getNumbersAndOcupancyTable(input);

    auto gearSum = 0ull;
    for (int row = 0; row < input.size(); ++row)
    {
        for (int col = 0; col < input[0].size(); ++col)
        {
            if (input[row][col] == '*')
            {
                std::set<int> gearPartsIndexes{};
                fillGearPart(row - 1, col - 1, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row - 1, col, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row - 1, col + 1, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row + 1, col - 1, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row + 1, col, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row + 1, col + 1, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row, col - 1, numbersOccupancy, numbers, gearPartsIndexes);
                fillGearPart(row, col + 1, numbersOccupancy, numbers, gearPartsIndexes);

                if (gearPartsIndexes.size() == 2)
                {
                    gearSum += numbers[*gearPartsIndexes.begin()].number * numbers[*gearPartsIndexes.rbegin()].number;
                }
            }
        }
    }
    return gearSum;
}

}  // namespace year_2023::day3
