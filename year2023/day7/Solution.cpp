#include "Solution.hpp"

#include <iostream>
#include <optional>
#include <sstream>
#include <algorithm>
#include <ranges>

#include <parsers/parsers.hpp>


namespace year_2023::day7
{

std::istream& operator>> (std::istream& is, Card& card)
{
	char c;
	is >> c;
	if (c == 'T') { card.value = 10; return is; }
	if (c == 'J') { card.value = 11; return is; }
	if (c == 'Q') { card.value = 12; return is; }
	if (c == 'K') { card.value = 13; return is; }
	if (c == 'A') { card.value = 14; return is; }

	card.value = c - '0';

	return is;
}

std::istream& operator>> (std::istream& is, Hand& hand)
{
	is >> hand.cards[0] >> hand.cards[1] >> hand.cards[2] >> hand.cards[3] >> hand.cards[4] >> hand.bet;
	return is;
}

namespace
{

struct QuanityToValue
{
	std::pair<int, int> data_;
	int& quantity() { return data_.first;  }
	int& value() { return data_.second; }
	int quantity() const { return data_.first; }
	int value() const { return data_.second; }
};

bool operator<(const QuanityToValue& lhs, const QuanityToValue& rhs)
{
	return lhs.data_ < rhs.data_;
}

struct PreprocessedHand
{
	Hand hand;
	std::vector<QuanityToValue> cardQuantities;
	long long strenght;
};

bool operator<(const PreprocessedHand& lhs, const PreprocessedHand& rhs)
{
	return lhs.strenght < rhs.strenght;
}

std::optional<long long> calculateStrenghtForFiveOfAKind(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 1)
	{
		return {1000ll};
	}
	return {};
}

std::optional<long long > calculateStrenghtForFourOfAKind(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 2 && quantities.front().quantity() == 4)
	{
		return { 900ll };
	}
	return {};
}

std::optional<long long> calculateStrenghtForFullOfAKind(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 2 && quantities.front().quantity() == 3)
	{
		return { 800ll };
	}
	return {};
}

std::optional<long long> calculateStrenghtForFreeOfAKind(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 3 && quantities.front().quantity() == 3)
	{
		return { 700ll };
	}
	return {};
}

std::optional<long long> calculateStrenghtForTwoPairs(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 3 && quantities.front().quantity() == 2)
	{
		return { 600ll };
	}
	return {};
}

std::optional<long long> calculateStrenghtForPair(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 4 && quantities.front().quantity() == 2)
	{
		return { 500ll };
	}
	return {};
}

std::optional<long long> calculateStrenghtForHighCard(const std::vector<QuanityToValue>& quantities)
{
	if (quantities.size() == 5)
	{
		return { 0ll };
	}
	return {};
}

long long calculateStrenght(const std::vector<QuanityToValue>& quantities)
{
	std::optional <long long> strenght;
	if (not strenght) strenght = calculateStrenghtForFiveOfAKind(quantities);
	if (not strenght) strenght = calculateStrenghtForFourOfAKind(quantities);
	if (not strenght) strenght = calculateStrenghtForFullOfAKind(quantities);
	if (not strenght) strenght = calculateStrenghtForFreeOfAKind(quantities);
	if (not strenght) strenght = calculateStrenghtForTwoPairs(quantities);
	if (not strenght) strenght = calculateStrenghtForPair(quantities);
	if (not strenght) strenght = calculateStrenghtForHighCard(quantities);

	return *strenght;
}

std::vector <PreprocessedHand> preprocess(const std::vector<Hand>& input)
{
	std::vector <PreprocessedHand> hands;
	for (auto&& hand : input)
	{
		PreprocessedHand preprocessedHand{ hand, {} };
		for (auto&& card : hand.cards)
		{
			auto it = std::find_if(preprocessedHand.cardQuantities.begin(), preprocessedHand.cardQuantities.end(), [&card](auto&& el) { return el.value() == card.value; });
			if (it != std::end(preprocessedHand.cardQuantities))
			{
				++it->quantity();
			}
			else
			{
				QuanityToValue q;
				q.value() = card.value;
				q.quantity() = 1;
				preprocessedHand.cardQuantities.emplace_back(q);
			}
		}
		std::sort(preprocessedHand.cardQuantities.rbegin(), preprocessedHand.cardQuantities.rend());
		preprocessedHand.strenght = calculateStrenght(preprocessedHand.cardQuantities);
		hands.push_back(preprocessedHand);
	}
	return hands;
}

std::vector <PreprocessedHand> preprocessWithJacksAsJockers(const std::vector<Hand>& input)
{
	std::vector <PreprocessedHand> hands;
	for (auto&& hand : input)
	{
		PreprocessedHand preprocessedHand{ hand, {} };
		auto jacks = 0;
		for (auto&& card : preprocessedHand.hand.cards)
		{
			if (card.value == 11)
			{
				++jacks;
				card.value = 1;
				continue;
			}
			auto it = std::find_if(preprocessedHand.cardQuantities.begin(), preprocessedHand.cardQuantities.end(), [&card](auto&& el) { return el.value() == card.value; });
			if (it != std::end(preprocessedHand.cardQuantities))
			{
				++it->quantity();
			}
			else
			{
				QuanityToValue q;
				q.value() = card.value;
				q.quantity() = 1;
				preprocessedHand.cardQuantities.emplace_back(q);
			}
		}
		std::sort(preprocessedHand.cardQuantities.rbegin(), preprocessedHand.cardQuantities.rend());
		if (jacks == 5)
		{
			QuanityToValue q;
			q.value() = 1;
			q.quantity() = 5;
			preprocessedHand.cardQuantities.emplace_back(q);
		}
		else
		{
			preprocessedHand.cardQuantities.front().quantity() += jacks;
		}

		preprocessedHand.strenght = calculateStrenght(preprocessedHand.cardQuantities);
		hands.push_back(preprocessedHand);
	}
	return hands;
}

} // namespace

std::vector<Hand> parse()
{
	auto file = parsers::getFile(2023, 7);
	std::vector<Hand> hands;

	for (auto&& line : file)
	{
		Hand h{};
		h.line = line;
		std::stringstream ss(line);
		ss >> h;
		hands.emplace_back(h);
	}
	return hands;
}

long long Solution::solve(const std::vector<Hand>& input) const
{
	auto preprocessedHands = preprocess(input);
	std::sort(preprocessedHands.begin(), preprocessedHands.end(), [](const auto& lhs, const auto& rhs) {
		return std::make_tuple(lhs.strenght, lhs.hand.cards[0].value, lhs.hand.cards[1].value, lhs.hand.cards[2].value, lhs.hand.cards[3].value, lhs.hand.cards[4].value)
			 < std::make_tuple(rhs.strenght, rhs.hand.cards[0].value, rhs.hand.cards[1].value, rhs.hand.cards[2].value, rhs.hand.cards[3].value, rhs.hand.cards[4].value);
		});

	long long res = 0ll;
	for (auto i = 0ll; i < preprocessedHands.size(); ++i)
	{
		res += preprocessedHands[i].hand.bet * (i + 1);
	}
	return res;
}

long long Solution::solve_part2(const std::vector<Hand>& input) const
{
	auto preprocessedHands = preprocessWithJacksAsJockers(input);
	std::sort(preprocessedHands.begin(), preprocessedHands.end(), [](const auto& lhs, const auto& rhs) {
		return std::make_tuple(lhs.strenght, lhs.hand.cards[0].value, lhs.hand.cards[1].value, lhs.hand.cards[2].value, lhs.hand.cards[3].value, lhs.hand.cards[4].value)
			< std::make_tuple(rhs.strenght, rhs.hand.cards[0].value, rhs.hand.cards[1].value, rhs.hand.cards[2].value, rhs.hand.cards[3].value, rhs.hand.cards[4].value);
		});
	
	long long res = 0ll;
	for (auto i = 0ll; i < preprocessedHands.size(); ++i)
	{
		res += preprocessedHands[i].hand.bet * (i + 1);
	}
	return res;
}

}  // namespace year_2023::day7
