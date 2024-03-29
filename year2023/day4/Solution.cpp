#include "Solution.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <numeric>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day4
{

std::vector<Game> parse()
{
	auto range = parsers::getFile(2023, 4);
	std::vector<Game> games;

	for (auto&& line : range)
	{
		Game game;
		auto gameToNumbers = splitAndTrim(line, ':');

		auto winningAndMy = splitAndTrim(gameToNumbers[1], '|');
        auto winningStr = splitAndTrim(winningAndMy[0], ' ');
		auto winningRanage = winningStr
			| std::views::filter([](auto s) {return not s.empty(); })
			| std::views::transform([](auto&& e) { return std::stoi(e); });
		std::copy(winningRanage.begin(), winningRanage.end(), std::inserter(game.winningNumbers_, std::begin(game.winningNumbers_)));

        auto myStr = splitAndTrim(winningAndMy[1], ' ');
		auto myRange = myStr
			| std::views::filter([](auto s) {return not s.empty(); })
			| std::views::transform([](auto&& e) { return std::stoi(e); });
		std::copy(myRange.begin(), myRange.end(), std::inserter(game.myNumbers_, std::begin(game.myNumbers_)));

		games.push_back(game);
	}
	return games;
}

static int getCommonNumberCount(const std::set<int>& lhs, const std::set<int>& rhs)
{
	std::vector<int> commonNumbers;
	std::set_intersection(lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end(),
		std::back_inserter(commonNumbers));
	return commonNumbers.size();
}

long long Solution::solve(const std::vector<Game>& input) const
{
	auto total = 0ull;
	for (auto&& game : input)
	{
		auto commonNumbers = getCommonNumberCount(game.myNumbers_, game.winningNumbers_);
		if (commonNumbers)
		{
			total += (1u << (commonNumbers - 1));
		}
	}
	
	return total;
}

long long Solution::solve_part2(const std::vector<Game>& input) const
{
	std::vector<long long> cardQuantities(input.size(), 1ull);
	for (auto i = 0u; i < input.size(); ++i)
	{
		auto& game = input[i];

		auto commonNumbers = getCommonNumberCount(game.myNumbers_, game.winningNumbers_);
		for (auto j = i + 1; j <= i + commonNumbers && j < input.size(); ++j)
		{
			cardQuantities[j] += cardQuantities[i];
		}
	}
	return std::accumulate(cardQuantities.begin(), cardQuantities.end(), 0ull);
}

}  // namespace year_2023::day4
