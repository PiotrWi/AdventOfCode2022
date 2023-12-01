#include "execute.hpp"

#include <cassert>

#include "utility/RaiiTimer.hpp"
#include "utility/SollutionExecucor.hpp"

#include "day1/Solution.hpp"

namespace year_2023
{

    void execute()
    {
        RaiiTimer rt("ALL TASKS from 2022");

        PrintAndVerifySollution<day1::Solution>()(day1::parse, "day 1", 55172, 54925);
    }

}  // namespace year_2023
