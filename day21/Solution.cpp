#include "Solution.hpp"

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace day21
{

const char *fileLoc = "day21/input.txt";

namespace
{

std::unique_ptr<Monkey> parseLine(const std::string& line)
{
    auto nameAndEquation = splitAndTrim(line, ':');
    auto equation = nameAndEquation[1];

    if (nameAndEquation[0] == "root")
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '+');
        auto monkey = std::make_unique<Root>();
        monkey->name_ = nameAndEquation[0];
        monkey->monkeySourceA_ = monkeys[0];
        monkey->monkeySourceB_ = monkeys[1];
        return monkey;
    }
    if (nameAndEquation[0] == "humn")
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '+');
        auto monkey = std::make_unique<Humman>();
        monkey->name_ = nameAndEquation[0];
        return monkey;
    }
    if (equation.find('+') != std::string::npos)
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '+');
        auto monkey = std::make_unique<MonkeyWithOperation>();
        monkey->name_ = nameAndEquation[0];
        monkey->monkeySourceA_ = monkeys[0];
        monkey->monkeySourceB_ = monkeys[1];
        monkey->operation_ = std::make_unique<Addition>();
        return monkey;
    }
    else if (equation.find('-') != std::string::npos)
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '-');
        auto monkey = std::make_unique<MonkeyWithOperation>();
        monkey->name_ = nameAndEquation[0];
        monkey->monkeySourceA_ = monkeys[0];
        monkey->monkeySourceB_ = monkeys[1];
        monkey->operation_ = std::make_unique<Substraction>();
        return monkey;
    }
    else if (equation.find('/') != std::string::npos)
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '/');
        auto monkey = std::make_unique<MonkeyWithOperation>();
        monkey->name_ = nameAndEquation[0];
        monkey->monkeySourceA_ = monkeys[0];
        monkey->monkeySourceB_ = monkeys[1];
        monkey->operation_ = std::make_unique<Divide>();
        return monkey;
    }
    else if (equation.find('*') != std::string::npos)
    {
        auto monkeys = splitAndTrim(nameAndEquation[1], '*');
        auto monkey = std::make_unique<MonkeyWithOperation>();
        monkey->name_ = nameAndEquation[0];
        monkey->monkeySourceA_ = monkeys[0];
        monkey->monkeySourceB_ = monkeys[1];
        monkey->operation_ = std::make_unique<Mutliply>();
        return monkey;
    }
    else
    {
        auto monkey = std::make_unique<MonkeyWithNumber>();
        monkey->name_ = nameAndEquation[0];
        monkey->number_ = std::stoi(nameAndEquation[1]);
        return monkey;
    }
    throw 1;
}

}  // namespace

std::map<std::string, std::unique_ptr<Monkey> > parse()
{
    std::map<std::string, std::unique_ptr<Monkey>> monkeys;
    for (auto&& line : parsers::LinesInFileRange(fileLoc))
    {
        auto monkey = parseLine(line);
        monkeys.emplace(monkey->name_, std::move(monkey));
    }
    return monkeys;
}

std::optional<long> MonkeyWithNumber::getValue(std::map<std::string, std::unique_ptr<Monkey> >&)
{
    return number_;
}

std::optional<long> MonkeyWithOperation::getValue(std::map<std::string, std::unique_ptr<Monkey> >& allMonkeys)
{
    if (number_)
    {
        return *number_;
    }
    auto& monkeyA = allMonkeys[monkeySourceA_];
    auto& monkeyB = allMonkeys[monkeySourceB_];

    valueFromA_ = monkeyA->getValue(allMonkeys);
    valueFromB_ = monkeyB->getValue(allMonkeys);

    if (valueFromA_ && valueFromB_)
    {
        number_ = (*operation_)(*valueFromA_, *valueFromB_);
    }

    return number_;
}

void MonkeyWithOperation::propagateUp(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys, std::optional<long> val)
{
    number_ = val;
    if (not valueFromA_)
    {
        auto a = operation_->calculateOpositeForA(*val, *valueFromB_);
        allMonkeys[monkeySourceA_]->propagateUp(allMonkeys, a);
    }
    if (not valueFromB_)
    {
        auto b = operation_->calculateOpositeForb(*val, *valueFromA_);
        allMonkeys[monkeySourceB_]->propagateUp(allMonkeys, b);
    }
}

std::optional<long> Root::getValue(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys)
{
    if (number_)
    {
        return *number_;
    }
    auto& monkeyA = allMonkeys[monkeySourceA_];
    auto& monkeyB = allMonkeys[monkeySourceB_];

    valueFromA_ = monkeyA->getValue(allMonkeys);
    valueFromB_ = monkeyB->getValue(allMonkeys);

    if (valueFromA_)
    {
        number_ = *valueFromA_;
    }

    if (valueFromB_)
    {
        number_ = *valueFromB_;
    }
    return number_;
}

void Root::propagateUp(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys, std::optional<long>)
{
    if (not valueFromA_)
    {
        allMonkeys[monkeySourceA_]->propagateUp(allMonkeys, *number_);
    }
    if (not valueFromB_)
    {
        allMonkeys[monkeySourceB_]->propagateUp(allMonkeys, *number_);
    }
}

std::optional<long> Humman::getValue(std::map<std::string, std::unique_ptr<Monkey>> &)
{
    return number_;
}

void Humman::propagateUp(std::map<std::string, std::unique_ptr<Monkey>> &, std::optional<long> val)
{
    number_ = val;
}

long Solution::solve(std::map<std::string, std::unique_ptr<Monkey>>& monkeys)
{
    return *monkeys["root"]->getValue(monkeys);
}

long Solution::solve_part2(std::map<std::string, std::unique_ptr<Monkey>>& monkeys)
{
    monkeys["root"]->getValue(monkeys);
    monkeys["root"]->propagateUp(monkeys, {});
    return *monkeys["humn"]->getValue(monkeys);
}

}
