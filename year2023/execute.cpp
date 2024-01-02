#include "execute.hpp"

#include <cassert>

#include "utility/RaiiTimer.hpp"
#include "utility/SollutionExecucor.hpp"

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
#include "day16/Solution.hpp"
#include "day17/Solution.hpp"
#include "day18/Solution.hpp"
#include "day19/Solution.hpp"
#include "day20/Solution.hpp"
#include "day21/Solution.hpp"
#include "day22/Solution.hpp"
#include "day23/Solution.hpp"
#include "day24/Solution.hpp"
#include "day25/Solution.hpp"

namespace year_2023
{

    void execute()
    {
        RaiiTimer rt("ALL TASKS from 2023");

        PrintAndVerifySollution<day1::Solution>()(day1::parse, "day 1", 55172, 54925);
        PrintAndVerifySollution<day2::Solution>()(day2::parse, "day 2", 2541, 66016);
        PrintAndVerifySollution<day3::Solution>()(day3::parse, "day 3", 535078, 75312571);
        PrintAndVerifySollution<day4::Solution>()(day4::parse, "day 4", 23441, 5923918);
        PrintAndVerifySollution<day5::Solution>()(day5::parse, "day 5", 457535844ll, 41222968);
        PrintAndVerifySollution<day6::Solution>()(day6::parse, "day 6", 275724, 37286485);
        PrintAndVerifySollution<day7::Solution>()(day7::parse, "day 7", 250474325, 248909434);
        PrintAndVerifySollution<day8::Solution>()(day8::parse, "day 8", 22199, 13334102464297ll);
        PrintAndVerifySollution<day9::Solution>()(day9::parse, "day 9", 1581679977, 889);
        PrintAndVerifySollution<day10::Solution>()(day10::parse, "day 10", 6956, 455);
        PrintAndVerifySollution<day11::Solution>()(day11::parse, "day 11", 9591768, 746962097860);
        PrintAndVerifySollution<day12::Solution>()(day12::parse, "day 12", 7173, 29826669191291ll);
        PrintAndVerifySollution<day13::Solution>()(day13::parse, "day 13", 33047, 28806);
        PrintAndVerifySollution<day14::Solution>()(day14::parse, "day 14", 105003ll, 93742ll);
        PrintAndVerifySollution<day15::Solution>()(day15::parse, "day 15", 516070ll, 244981ll);
        PrintAndVerifySollution<day16::Solution>()(day16::parse, "day 16", 7060ll, 7493ll);
        PrintAndVerifySollution<day17::Solution>()(day17::parse, "day 17", 866ll, 1010ll);
        PrintAndVerifySollution<day18::Solution>()(day18::parse, "day 18", 46359ll, 59574883048274ll);
        PrintAndVerifySollution<day19::Solution>()(day19::parse, "day 19", 397061ll, 125657431183201ll);
        PrintAndVerifySollution<day20::Solution>()(day20::parse, "day 20", 980457412ll, 232774988886497ll);
        PrintAndVerifySollution<day21::Solution>()(day21::parse, "day 21", 3853, 639051580070841);
        PrintAndVerifySollution<day22::Solution>()(day22::parse, "day 22", 490, 96356);
        PrintAndVerifySollution<day23::Solution>()(day23::parse, "day 23", 2310, 6738);
        PrintAndVerifySollution<day24::Solution>()(day24::parse, "day 24", 12343ll, 769281292688187ll);
        PrintAndVerifySollution<day25::Solution>()(day25::parse, "day 25", 543256ll);
    }

}  // namespace year_2023
