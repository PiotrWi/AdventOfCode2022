#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <optional>

namespace day21
{

struct Operation
{
    virtual long long operator()(long long lhs, long long rhs) = 0;
    virtual long long calculateOpositeForA(long long val, long long a) = 0;
    virtual long long calculateOpositeForb(long long val, long long b) = 0;
    virtual ~Operation() {};
};

struct Addition : public Operation
{
    long long operator()(long long lhs, long long rhs) override { return lhs + rhs; }
    long long calculateOpositeForA(long long val, long long b) override { return val - b; }
    long long calculateOpositeForb(long long val, long long a) override { return val - a; }
};

struct Substraction : public Operation
{
    long long operator()(long long lhs, long long rhs) override { return lhs - rhs; }
    long long calculateOpositeForA(long long val, long long b) override { return val + b; }
    long long calculateOpositeForb(long long val, long long a) override { return a - val; }
};

struct Mutliply : public Operation
{
    long long operator()(long long lhs, long long rhs) override { return lhs * rhs; }
    long long calculateOpositeForA(long long val, long long b) override { return val / b; }
    long long calculateOpositeForb(long long val, long long a) override { return val / a; }
};

struct Divide : public Operation
{
    long long operator()(long long lhs, long long rhs) override { return lhs / rhs; }
    long long calculateOpositeForA(long long val, long long b) override { return b * val; }
    long long calculateOpositeForb(long long val, long long a) override { return a / val; }
};


struct Monkey
{
    virtual std::optional<long long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) = 0;
    virtual void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long long>) = 0;
    virtual ~Monkey() {}
    std::string name_;
};

struct MonkeyWithNumber : public Monkey
{
    std::optional<long long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long long>) override {};
    long long number_;
};

struct MonkeyWithOperation : public Monkey
{
    std::optional<long long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long long>) override;
    std::unique_ptr<Monkey> cloneToRoot();
    std::unique_ptr<Monkey> cloneToHumman();

    std::string monkeySourceA_;
    std::optional <long long> valueFromA_;
    std::string monkeySourceB_;
    std::optional <long long> valueFromB_;

    std::unique_ptr<Operation> operation_;
    std::optional<long long> number_;
};

struct Root : public MonkeyWithOperation
{
    std::optional<long long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long long>) override;
};

struct Humman : public Monkey
{
    std::optional<long long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long long>) override;
    std::optional<long long> number_;
};

std::map<std::string, std::unique_ptr<Monkey> > parse();

struct Solution
{
    long long solve(std::map<std::string, std::unique_ptr<Monkey> >&);
    long long solve_part2(std::map<std::string, std::unique_ptr<Monkey> >&);
};

}
