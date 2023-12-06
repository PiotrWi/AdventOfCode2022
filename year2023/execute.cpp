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
        PrintAndVerifySollution<day6::Solution>()(day6::parse, "day 6", NONE{}, NONE{});

    }

}  // namespace year_2023
