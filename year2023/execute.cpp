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

namespace year_2023
{

    void execute()
    {
        RaiiTimer rt("ALL TASKS from 2022");

        PrintAndVerifySollution<day1::Solution>()(day1::parse, "day 1", 55172, 54925);
        PrintAndVerifySollution<day2::Solution>()(day2::parse, "day 2", 2541, 66016);
        PrintAndVerifySollution<day3::Solution>()(day3::parse, "day 3", 535078, 75312571);
        PrintAndVerifySollution<day4::Solution>()(day4::parse, "day 4", 23441, 5923918);
        PrintAndVerifySollution<day5::Solution>()(day5::parse, "day 5", 457535844ll, 41222968);
        PrintAndVerifySollution<day6::Solution>()(day6::parse, "day 6", 275724, 37286485);
        PrintAndVerifySollution<day7::Solution>()(day7::parse, "day 7", 250474325, 248909434);
        PrintAndVerifySollution<day8::Solution>()(day8::parse, "day 8", 22199, 13334102464297ll);
        PrintAndVerifySollution<day9::Solution>()(day9::parse, "day 9", 1581679977, 889);
        PrintAndVerifySollution<day10::Solution>()(day10::parse, "day 10", NONE{}, NONE{});
        PrintAndVerifySollution<day11::Solution>()(day11::parse, "day 11", NONE{}, NONE{});

    }

}  // namespace year_2023
