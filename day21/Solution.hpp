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
    virtual long operator()(long lhs, long rhs) = 0;
    virtual long calculateOpositeForA(long val, long a) = 0;
    virtual long calculateOpositeForb(long val, long b) = 0;
    virtual ~Operation() {};
};

struct Addition : public Operation
{
    long operator()(long lhs, long rhs) override { return lhs + rhs; }
    long calculateOpositeForA(long val, long b) override{ return val - b; }
    long calculateOpositeForb(long val, long a) override{ return val - a; }
};

struct Substraction : public Operation
{
    long operator()(long lhs, long rhs) override { return lhs - rhs; }
    long calculateOpositeForA(long val, long b) override { return val + b; }
    long calculateOpositeForb(long val, long a) override { return a - val; }
};

struct Mutliply : public Operation
{
    long operator()(long lhs, long rhs) override { return lhs * rhs; }
    long calculateOpositeForA(long val, long b) override{ return val / b; }
    long calculateOpositeForb(long val, long a) override{ return val / a; }
};

struct Divide : public Operation
{
    long operator()(long lhs, long rhs) override { return lhs / rhs; }
    long calculateOpositeForA(long val, long b) override {return b * val; }
    long calculateOpositeForb(long val, long a) override {return a / val; }
};


struct Monkey
{
    virtual std::optional<long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) = 0;
    virtual void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long>) = 0;
    virtual ~Monkey() {}
    std::string name_;
};

struct MonkeyWithNumber : public Monkey
{
    std::optional<long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long>) override {};
    long number_;
};

struct MonkeyWithOperation : public Monkey
{
    std::optional<long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long>) override;
    std::string monkeySourceA_;
    std::optional <long> valueFromA_;
    std::string monkeySourceB_;
    std::optional <long> valueFromB_;

    std::unique_ptr<Operation> operation_;
    std::optional<long> number_;
};

struct Root : public MonkeyWithOperation
{
    std::optional<long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long>) override;
};

struct Humman : public Monkey
{
    std::optional<long> getValue(std::map<std::string, std::unique_ptr<Monkey> >&) override;
    void propagateUp(std::map<std::string, std::unique_ptr<Monkey> >&, std::optional<long>) override;
    std::optional<long> number_;
};

std::map<std::string, std::unique_ptr<Monkey> > parse();

struct Solution
{
    long solve(std::map<std::string, std::unique_ptr<Monkey> >&);
    long solve_part2(std::map<std::string, std::unique_ptr<Monkey> >&);
};

}
