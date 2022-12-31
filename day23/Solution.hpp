#pragma once

#include <vector>

namespace day23
{

using ElfsSurface = std::vector<std::vector<char>>;

ElfsSurface parse();

struct Solution
{
    int solve(const ElfsSurface&);
    int solve_part2(const ElfsSurface&);
};

}  // namespace day23

