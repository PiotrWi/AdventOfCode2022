#include <iostream>

#include "day1/sollution.hpp"
#include "day2/sollution.hpp"

int main()
{
    {
        day1::Sollution s;
        auto intput = day1::parse();
        std::cout << "day 1 part 1: " << s.getSollution(intput) << std::endl;
        std::cout << "day 1 part 2: " << s.getSollution_part2(intput) << std::endl;
    }
    {
        day2::Sollution s;
        auto intput = day2::parse();
        std::cout << "day 2 test 1: " << s.getSollution({{'A', 'Y'}, {'B', 'X'}, {'C', 'Z'}}) << std::endl;
        std::cout << "day 2 part 1: " << s.getSollution(intput) << std::endl;
        std::cout << "day 2 part 2: " << s.getSollution_part2(intput) << std::endl;

    }

    return 0;
}
