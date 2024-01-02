#include "execute.hpp"

#include <cassert>

#include "day1/Solution.hpp"
#include "day2/Solution.hpp"
#include "day3/Solution.hpp"
#include "day4/Solution.hpp"
#include "day5/Solution.hpp"
#include "day6/Solution.hpp"
#include "day7/Solution.hpp"
#include "day8/Solution.hpp"
#include "day9/Solution.hpp"
#include "day10/Solution.hpp"
#include "day11/Solution.hpp"
#include "day12/Solution.hpp"
#include "day13/Solution.hpp"
#include "day14/Solution.hpp"
#include "day15/Solution.hpp"
// #include "day16/Solution.hpp"
#include "day17/Solution.hpp"
#include "day18/Solution.hpp"
#include "day19/Solution.hpp"
#include "day20/Solution.hpp"
#include "day21/Solution.hpp"
#include "day22/Solution.hpp"
#include "day23/Solution.hpp"
#include "day24/Solution.hpp"
#include "day25/Solution.hpp"
#include "utility/RaiiTimer.hpp"
#include "utility/SollutionExecucor.hpp"

namespace year_2022
{

void execute()
{
    RaiiTimer rt("ALL TASKS from 2022");

    PrintAndVerifySollution<day1::Solution>()(day1::parse, "day 1", 71502, 208191);
    PrintAndVerifySollution<day2::Solution>()(day2::parse, "day 2", 13484, 13433);
    PrintAndVerifySollution<day3::Solution>()(day3::parse, "day 3", 7821, 2752);
    PrintAndVerifySollution<day4::Solution>()(day4::parse, "day 4", 471, 888);
    PrintAndVerifySollution<day5::Solution>()(day5::parse, "day 5", "ZBDRNPMVH", "WDLPFNNNB");
    PrintAndVerifySollution<day6::Solution>()(day6::parse, "day 6", 1909, 3380);
    PrintAndVerifySollution<day7::Solution>()(day7::parse, "day 7", 1642503, 6999588);
    PrintAndVerifySollution<day8::Solution>()(day8::parse, "day 8", 1719, 590824);
    PrintAndVerifySollution<day9::Solution>()(day9::parse, "day 9", 6337, 2455);
    PrintAndVerifySollution<day10::Solution>()(day10::parse, "day 10", 14040, SolveBWithoutCheck{});
    PrintAndVerifySollution<day11::Solution>()(day11::parse, "day 11", 76728ull, 21553910156ull);
    PrintAndVerifySollution<day12::Solution>()(day12::parse, "day 12", 339, 332);
    PrintAndVerifySollution<day13::Solution>()(day13::parse, "day 13", 5393, 26712);
    PrintAndVerifySollution<day14::Solution>()(day14::parse, "day 14", 768, 26686);
    PrintAndVerifySollution<day15::Solution>()(day15::parse, "day 15", 5108096, 10553942650264ull);  // b can be improved
    // PrintAndVerifySollution<day16::Solution>()(day16::parse, "day 16", 1647, 2169); // Temporarly disabled due fact that intrincs are not available on all targets. std bit operations to be used.
    PrintAndVerifySollution<day17::Solution>()(day17::parse, "day 17", 3100, 1540634005751);
    PrintAndVerifySollution<day18::Solution>()(day18::parse, "day 18", 3498, 2008);
    PrintAndVerifySollution<day19::Solution>()(day19::parse, "day 19", 1404, 5880);
    PrintAndVerifySollution<day20::Solution>()(day20::parse, "day 20", 4224, 861907680486);
    PrintAndVerifySollution<day21::Solution>()(day21::parse, "day 21", 82225382988628ll, 3429411069028ll);
    PrintAndVerifySollution<day22::Solution>()(day22::parse, "day 22", 60362, 74288);
    PrintAndVerifySollution<day23::Solution>()(day23::parse, "day 23", 4241, 1079);
    PrintAndVerifySollution<day24::Solution>()(day24::parse, "day 24", 225, 711);
    PrintAndVerifySollution<day25::Solution>()(day25::parse, "day 25", "2-121-=10=200==2==21");
}

}  // namespace year_2022
