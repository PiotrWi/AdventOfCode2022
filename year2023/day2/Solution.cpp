#include "Solution.hpp"

#include <algorithm>
#include <numeric>
#include <sstream>

#include <parsers/parsers.hpp>
#include <StringAlgorithms/StringAlgorithms.hpp>

namespace year_2023::day2
{

namespace
{

const char* fileLoc = "year2023/day2/input.txt";

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
	std::vector<BallsRecordEntity> out;
	std::ranges::transform(splitAndTrim(recordsLine, ';'), std::back_inserter(out), parseRecord);
	return out;
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
	auto range = parsers::LinesInFileRange(fileLoc);
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
	int out = 0;
	for (int i = 1; i <= input.size(); ++i)
	{
		if (matchRecords(input[i - 1])) out += i;
	}
	return out;
}

int Solution::solve_part2(const std::vector<GameRecord>& input) const
{
	int power = 0;
	std::vector<int> powers;
	std::transform(input.begin(), input.end(), std::back_inserter(powers), calculatePower);
	power = std::accumulate(powers.begin(), powers.end(), power);
	return power;
}

}  // namespace year_2023::day2
