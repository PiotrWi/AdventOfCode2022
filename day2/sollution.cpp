#include "sollution.hpp"
#include <fstream>

enum class hand
{
    rock,
    paper,
    scissors,
};

enum class Result
{
    lost,
    draw,
    win,
};

namespace day2
{
const char* fileLoc = "day2/input.txt";

std::vector<std::pair<char, char>> parse()
{
    std::vector<std::pair<char, char>> out;
    std::fstream inputFile(fileLoc);
    std::string line;

    std::pair<char, char> singleInput;
    while (inputFile)
    {
        std::getline(inputFile, line);
        if (line.size() >= 3)
        {
            singleInput.first = line[0];
            singleInput.second = line[2];
            out.emplace_back(singleInput);
        }
    }

    inputFile.close();
    return out;
}

namespace
{

int getPoints(const std::pair<hand, hand>& in)
{
    int out = 0;
    if (in.first == hand::rock && in.second == hand::paper)
        out += 6;
    if (in.first == hand::rock && in.second == hand::rock)
        out += 3;
    if (in.first == hand::paper && in.second == hand::scissors)
        out += 6;
    if (in.first == hand::paper && in.second == hand::paper)
        out += 3;
    if (in.first == hand::scissors && in.second == hand::rock)
        out += 6;
    if (in.first == hand::scissors && in.second == hand::scissors)
        out += 3;

    if (in.second == hand::rock)
        out += 1;
    if (in.second == hand::paper)
        out += 2;
    if (in.second == hand::scissors)
        out += 3;

    return  out;
}

auto map_char_to_hand(char in)
{
    if (in == 'A')
    {
        return hand::rock;
    }
    if (in == 'B')
    {
        return hand::paper;
    }
    if (in == 'C')
    {
        return hand::scissors;
    }
    if (in == 'X')
    {
        return hand::rock;
    }
    if (in == 'Y')
    {
        return hand::paper;
    }
    return hand::scissors;

}

auto map_char_to_result(char in)
{
    if (in == 'X')
    {
        return Result::lost;
    }
    if (in == 'Y')
    {
        return Result::draw;
    }
    return Result::win;
}

std::pair<hand, hand> map_win_lose_to_hands(const std::pair<hand, Result>& in)
{
    std::pair<hand, hand> out;
    out.first = in.first;
    if (in.first == hand::rock and in.second == Result::lost)
    {
        out.second = hand::scissors;
    }
    if (in.first == hand::rock and in.second == Result::draw)
    {
        out.second = hand::rock;
    }
    if (in.first == hand::rock and in.second == Result::win)
    {
        out.second = hand::paper;
    }
    if (in.first == hand::paper and in.second == Result::lost)
    {
        out.second = hand::rock;
    }
    if (in.first == hand::paper and in.second == Result::draw)
    {
        out.second = hand::paper;
    }
    if (in.first == hand::paper and in.second == Result::win)
    {
        out.second = hand::scissors;
    }
    if (in.first == hand::scissors and in.second == Result::lost)
    {
        out.second = hand::paper;
    }
    if (in.first == hand::scissors and in.second == Result::draw)
    {
        out.second = hand::scissors;
    }
    if (in.first == hand::scissors and in.second == Result::win)
    {
        out.second = hand::rock;
    }
    return out;
}

}  // namespace

int Sollution::getSollution(const std::vector<std::pair<char, char>>& input)
{
    int out = 0;
    for (auto&& in: input)
    {
        auto singleHand = std::pair<hand, hand>{map_char_to_hand(in.first), map_char_to_hand(in.second)};
        out += getPoints(singleHand);
    }
    return out;
}

// x - lose, y - draw, z - win
int Sollution::getSollution_part2(const std::vector<std::pair<char, char>>& input)
{
    int out = 0;
    for (auto&& in: input)
    {
        auto handToResult = std::pair<hand, Result>{map_char_to_hand(in.first), map_char_to_result(in.second)};
        auto singleHand = map_win_lose_to_hands(handToResult);
        out += getPoints(singleHand);
    }
    return out;
}

}  // namespace day2
