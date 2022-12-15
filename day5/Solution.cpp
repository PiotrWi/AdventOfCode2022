#include "Solution.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace day5
{
const char* fileLoc = "day5/input.txt";

namespace
{

void parseStackLine(Input& in, const std::string &line)
{
    for (unsigned int i = 1; i < line.size(); i += 4) {
        if (line[i] != ' ') {
            in.stacks[i / 4].emplace_back(line[i]);
        }
    }
}

Move parseToCommand(const std::string &line)
{
    std::string ignore;
    Move out;
    std::stringstream ss(line);
    ss >> ignore >> out.count >> ignore >> out.from >> ignore >> out.to;
    return out;
}

}  // namespace


Input parse()
{
    Input input;

    std::fstream inputFile(fileLoc);
    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line == "")
            break;
        if (line.find('[') == std::string::npos)
        {
            continue;
        }
        parseStackLine(input, line);
    }

    for (auto& stack_: input.stacks)
    {
        std::reverse(stack_.begin(), stack_.end());
    }

    while (std::getline(inputFile, line))
    {
        input.commands.emplace_back(parseToCommand(line));
    }
    inputFile.close();

    return input;
}

std::string Solution::solve(Input input)
{
    std::string out;
    for (auto&& command : input.commands)
    {
        auto count = command.count;
        auto& sourceStack = input.stacks[command.from-1];
        auto& destinationStack = input.stacks[command.to-1];
        destinationStack.insert(destinationStack.end(), sourceStack.rbegin(), sourceStack.rbegin() + count);
        sourceStack.erase(sourceStack.end()-count, sourceStack.end());
    }
    for (auto&& stack : input.stacks)
    {
        out.push_back(stack.back());
    }
    return out;
}

std::string Solution::solve_part2(Input input)
{
    std::string out;
    for (auto&& command : input.commands)
    {
        auto count = command.count;
        auto& sourceStack = input.stacks[command.from-1];
        auto& destinationStack = input.stacks[command.to-1];
        destinationStack.insert(destinationStack.end(), sourceStack.end() - count, sourceStack.end());
        sourceStack.erase(sourceStack.end()-count, sourceStack.end());
    }
    for (auto&& stack : input.stacks)
    {
        out.push_back(stack.back());
    }
    return out;
}

}  // namespace day5
