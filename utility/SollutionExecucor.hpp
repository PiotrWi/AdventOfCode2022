#include <iostream>
#include <cassert>


struct NONE {};
struct SolveBWithoutCheck {};

template <typename T>
bool operator == (T&&, const NONE&)
{
    return true;
}

template <typename TSolutionClass>
struct PrintAndVerifySollution
{
    template<typename T, typename ARetType, typename BRetType>
    void operator()(T(&parseFcn)(),
        const char* prefix,
        [[maybe_unused]] const ARetType& expectationA,
        [[maybe_unused]] const BRetType& expectationb)
    {
        RaiiTimer rt(prefix);

        auto intput = parseFcn();
        auto solver = TSolutionClass{};

        auto solutionA = solver.solve(intput);
        std::cout << prefix << " part 1: " << solutionA << std::endl;
        assert(expectationA == solutionA);

        auto solutionB = solver.solve_part2(intput);
        std::cout << prefix << " part 2: " << solutionB << std::endl;
        assert(solutionB == expectationb);
    }

    template<typename T, typename ARetType>
    void operator()(T(&parseFcn)(),
        const char* prefix,
        [[maybe_unused]] const ARetType& expectationA)
    {
        RaiiTimer rt(prefix);

        auto intput = parseFcn();
        auto solver = TSolutionClass{};

        auto solutionA = solver.solve(intput);
        assert(expectationA == solutionA);
        std::cout << prefix << " part 1: " << solutionA << std::endl;
    }

    template<typename T, typename ARetType>
    void operator()(T(&parseFcn)(),
        const char* prefix,
        [[maybe_unused]] const ARetType& expectationA,
        SolveBWithoutCheck)
    {
        RaiiTimer rt(prefix);        auto intput = parseFcn();
        auto solver = TSolutionClass{};

        auto solutionA = solver.solve(intput);
        assert(expectationA == solutionA);
        std::cout << prefix << " part 1: " << solutionA << std::endl;

        solver.solve_part2(intput);
    }
};