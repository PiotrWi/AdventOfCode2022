#include "Solution.hpp"

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace day21
{

namespace
{

std::unique_ptr<Operation> toOperation(const char sign)
{
    if (sign == '+') return std::make_unique<Addition>();
    if (sign == '-') return std::make_unique<Substraction>();
    if (sign == '/') return std::make_unique<Divide>();
    if (sign == '*') return std::make_unique<Mutliply>();
    throw 1;
}

std::unique_ptr<Monkey> monkeyWithOperation(const std::string& name, const std::string& equation, const char sign)
{
    auto monkeys = splitAndTrim(equation, sign);
    auto monkey = std::make_unique<MonkeyWithOperation>();
    monkey->name_ = name;
    monkey->monkeySourceA_ = monkeys[0];
    monkey->monkeySourceB_ = monkeys[1];
    monkey->operation_ = toOperation(sign);
    return monkey;
}

std::unique_ptr<Monkey> parseLine(const std::string& line)
{
    auto nameAndEquation = splitAndTrim(line, ':');
    auto equation = nameAndEquation[1];

    for (char c : {'+', '-', '/', '*' })
    {
        if (equation.find(c) != std::string::npos)
        {
            return monkeyWithOperation(nameAndEquation[0], equation, c);
        }
    }

    auto monkey = std::make_unique<MonkeyWithNumber>();
    monkey->name_ = nameAndEquation[0];
    monkey->number_ = std::stoi(nameAndEquation[1]);
    return monkey;
}

}  // namespace

std::map<std::string, std::unique_ptr<Monkey> > parse()
{
    std::map<std::string, std::unique_ptr<Monkey>> monkeys;
    for (auto&& line : parsers::getFile(2023, 21))
    {
        auto monkey = parseLine(line);
        monkeys.emplace(monkey->name_, std::move(monkey));
    }
    return monkeys;
}

std::optional<long long> MonkeyWithNumber::getValue(std::map<std::string, std::unique_ptr<Monkey> >&)
{
    return number_;
}

std::optional<long long> MonkeyWithOperation::getValue(std::map<std::string, std::unique_ptr<Monkey> >& allMonkeys)
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

void MonkeyWithOperation::propagateUp(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys, std::optional<long long> val)
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

std::unique_ptr<Monkey> MonkeyWithOperation::cloneToRoot()
{
    auto monkey = std::make_unique<Root>();
    monkey->name_ = name_;
    monkey->monkeySourceA_ = monkeySourceA_;
    monkey->monkeySourceB_ = monkeySourceB_;
    return monkey;
}

std::unique_ptr<Monkey> MonkeyWithOperation::cloneToHumman()
{
    auto monkey = std::make_unique<Humman>();
    monkey->name_ = name_;
    return monkey;
}

std::optional<long long> Root::getValue(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys)
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

void Root::propagateUp(std::map<std::string, std::unique_ptr<Monkey>>& allMonkeys, std::optional<long long>)
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

std::optional<long long> Humman::getValue(std::map<std::string, std::unique_ptr<Monkey>> &)
{
    return number_;
}

void Humman::propagateUp(std::map<std::string, std::unique_ptr<Monkey>> &, std::optional<long long> val)
{
    number_ = val;
}

long long Solution::solve(std::map<std::string, std::unique_ptr<Monkey>>& monkeys)
{
    return *monkeys["root"]->getValue(monkeys);
}

long long Solution::solve_part2(std::map<std::string, std::unique_ptr<Monkey>>& monkeys)
{
    monkeys = parse();
    monkeys["root"] = static_cast<MonkeyWithOperation*>(monkeys["root"].get())->cloneToRoot();
    monkeys["humn"] = static_cast<MonkeyWithOperation*>(monkeys["humn"].get())->cloneToHumman();

    monkeys["root"]->getValue(monkeys);
    monkeys["root"]->propagateUp(monkeys, {});

    return *monkeys["humn"]->getValue(monkeys);
}

}
