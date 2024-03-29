#include "Solution.hpp"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <ranges>

#include <utility/RangesUtils.hpp>
#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day2
{

namespace
{

BallsRecordEntity parseRecord(const std::string& line)
{
	BallsRecordEntity entity;
	for (auto& numToBallStr: splitAndTrim(line, ','))
	{
		int num = 0;
		std::string color;
		std::stringstream ss(numToBallStr);
		ss >> num >> color;

		if (color == "red") entity.redBalls = num;
		if (color == "green") entity.greenBalls = num;
		if (color == "blue") entity.blueBalls = num;
	}
	return entity;
}

std::vector<BallsRecordEntity> parseRecords(const std::string& recordsLine)
{
    auto recordStr = splitAndTrim(recordsLine, ';');
	return recordStr | std::views::transform(parseRecord) | ToVector();
}

BallsRecordEntity getMaximumForEachColor(const GameRecord& record)
{
	BallsRecordEntity maximumValues;
	for (auto&& record : record.records)
	{
		maximumValues.redBalls = std::max(maximumValues.redBalls, record.redBalls);
		maximumValues.greenBalls = std::max(maximumValues.greenBalls, record.greenBalls);
		maximumValues.blueBalls = std::max(maximumValues.blueBalls, record.blueBalls);
	}
	return maximumValues;
}

bool operator<=(const BallsRecordEntity& lhs, const BallsRecordEntity& rhs)
{
	return lhs.redBalls <= rhs.redBalls
		&& lhs.greenBalls <= rhs.greenBalls
		&& lhs.blueBalls <= rhs.blueBalls;
}

bool matchRecords(const GameRecord& record)
{
	BallsRecordEntity maximumValues = getMaximumForEachColor(record);
	BallsRecordEntity limit;
	limit.redBalls = 12;
	limit.greenBalls = 13;
	limit.blueBalls = 14;

	return maximumValues <= limit;
}

int calculatePower(const GameRecord& record)
{
	BallsRecordEntity maximumValues = getMaximumForEachColor(record);
	return maximumValues.redBalls * maximumValues.greenBalls * maximumValues.blueBalls;
}

}  // namespace

std::vector<GameRecord> parse()
{
	std::vector<GameRecord> out;
	auto range = parsers::getFile(2023, 2);
	for (auto&& line : range)
	{
		auto gameAndRecords = splitAndTrim(line, ':');
		auto& records = gameAndRecords[1];

		out.emplace_back(parseRecords(records));
	}
	return out;
}

int Solution::solve(const std::vector<GameRecord>& input) const
{
	auto out = 0u;
	for (auto i = 1u; i <= input.size(); ++i)
	{
		if (matchRecords(input[i - 1])) out += i;
	}
	return out;
}

int Solution::solve_part2(const std::vector<GameRecord>& input) const
{
	auto powers = input | std::views::transform(calculatePower) | ToVector();

	int power = 0;
	power = std::accumulate(powers.begin(), powers.end(), power);
	return power;
}

}  // namespace year_2023::day2
