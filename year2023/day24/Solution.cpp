#include "Solution.hpp"

#include <ranges>
#include <iostream>
#include <map>
#include <variant>
#include <utility/RangesUtils.hpp>

#include "parsers/parsers.hpp"
#include "StringAlgorithms/StringAlgorithms.hpp"

namespace year_2023::day24
{

InputType parse()
{
	InputType in;
	for (auto&& line : parsers::getFile(2023, 24))
	{
		Cube c;
		auto positionToVelocity = splitAndTrim(line, '@');
		auto positions = splitAndTrim(positionToVelocity[0], ',') | std::views::transform([](auto&& str) { return std::stod(str); }) | To<std::vector<long long>>();
		c.position_.X_ = positions[0];
		c.position_.Y_ = positions[1];
		c.position_.Z_ = positions[2];

		auto velocites = splitAndTrim(positionToVelocity[1], ',') | std::views::transform([](auto&& str) { return std::stod(str); }) | To<std::vector<long long>>();
		c.velocity_.X_ = velocites[0];
		c.velocity_.Y_ = velocites[1];
		c.velocity_.Z_ = velocites[2];

		in.push_back(c);
	}

	return in;
}

bool isEqual(double lhs, double rhs, double epsilon = 0.001)
{
	return abs(lhs - rhs) < epsilon;
}

template <double RangeBegin, double RangeEnd>
bool inRange(double val, double epsilon = 0.001)
{
	return (val > (RangeBegin - epsilon)) && (val < (RangeEnd + epsilon));
}

bool isInFuture(double val_0, double val, double vel, double epsilon = 0.001)
{
	if (vel > 0)
	{
		return val > (val_0 - epsilon);
	}
	return val < (val_0 + epsilon);
}

template <double RangeBegin, double RangeEnd>
bool match(Cube lhs, Cube rhs, double epsilon = 0.001)
{
	auto a_lhs = (double)lhs.velocity_.Y_ / lhs.velocity_.X_;
	auto b_lhs = (double)lhs.position_.Y_ - a_lhs * lhs.position_.X_;

	auto a_rhs = (double)rhs.velocity_.Y_ / rhs.velocity_.X_;
	auto b_rhs = (double)rhs.position_.Y_ - a_rhs * rhs.position_.X_;

	if (a_lhs == a_rhs)
	{
		return b_lhs == b_rhs;
	}

	auto x = (b_rhs - b_lhs) / (a_lhs - a_rhs);
	auto y = a_lhs * x + b_lhs;

	return inRange<RangeBegin, RangeEnd>(x)
		&& inRange<RangeBegin, RangeEnd>(y)
		&& isInFuture((double)lhs.position_.X_, x, (double)lhs.velocity_.X_)
		&& isInFuture((double)rhs.position_.X_, x, (double)rhs.velocity_.X_);
}

long long Solution::solve(const InputType& input) const
{
	auto count = 0ll;
	for (auto lhs = input.begin(); lhs != input.end(); ++lhs)
	{
		for (auto rhs = lhs + 1; rhs != input.end(); ++rhs)
		{
			// count += match<7.0, 24.0>(*lhs, *rhs);  // test input
			count += match<200000000000000.0, 400000000000000.0>(*lhs, *rhs);
		}
	}
	return count;
}

struct Generator
{
	long long operator()()
	{
		return generate();
	}
	long long generate()
	{
		sign_ *= -1;
		++value_;
		return (value_ / 2) * sign_;
	}
	long long sign_ = -1;
	long long value_ = 1;
};

std::optional<std::pair<double, double>> calcCrossXZ(Cube lhs, Cube rhs)
{
	auto a_lhs = (double)lhs.velocity_.Z_ / lhs.velocity_.X_;
	auto b_lhs = (double)lhs.position_.Z_ - a_lhs * lhs.position_.X_;

	auto a_rhs = (double)rhs.velocity_.Z_ / rhs.velocity_.X_;
	auto b_rhs = (double)rhs.position_.Z_ - a_rhs * rhs.position_.X_;

	if (a_lhs == a_rhs)
	{
		return {}; // tmp
	}
	 
	auto x = (b_rhs - b_lhs) / (a_lhs - a_rhs);
	auto z = a_lhs * x + b_lhs;
	return std::make_pair(x, z);
}

bool tryForGivenVX_VZ(const InputType& input, long long VtrowX, long long VtrowZ)
{
	// std::cout << "try: " << VtrowX << ", " << VtrowY << std::endl;

	Cube c0 = input[0];
	c0.velocity_.X_ -= VtrowX;
	c0.velocity_.Z_ -= VtrowZ;

	double xPattern, zPattern;
	for (int i = 1; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.X_ -= VtrowX;
		c.velocity_.Z_ -= VtrowZ;
		auto xzCurrent = calcCrossXZ(c0, c);
		if (xzCurrent)
		{
			xPattern = xzCurrent->first;
			zPattern = xzCurrent->second;
			break;
		}
	}

	for (int i = 1; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.X_ -= VtrowX;
		c.velocity_.Z_ -= VtrowZ;
		auto xzCurrent = calcCrossXZ(c0, c);
		if (not xzCurrent)
		{
			continue;
		}
		if (not isEqual(xzCurrent->first, xPattern, 100.0) ||
			not isEqual(xzCurrent->second, zPattern, 100.0))
		{
			return false;
		}
	}
	std::cout << "Found: " << VtrowX << ", " << VtrowZ << std::endl;
	std::cout << (long long)xPattern << ", " << (long long)zPattern << std::endl;
	return true;
}

std::optional<std::pair<double, double>> calcCrossXY(Cube lhs, Cube rhs)
{
	auto a_lhs = (double)lhs.velocity_.Y_ / lhs.velocity_.X_;
	auto b_lhs = (double)lhs.position_.Y_ - a_lhs * lhs.position_.X_;

	auto a_rhs = (double)rhs.velocity_.Y_ / rhs.velocity_.X_;
	auto b_rhs = (double)rhs.position_.Y_ - a_rhs * rhs.position_.X_;

	if (a_lhs == a_rhs)
	{
		return {}; // tmp
	}

	auto x = (b_rhs - b_lhs) / (a_lhs - a_rhs);
	auto y = a_lhs * x + b_lhs;
	return std::make_pair(x, y);
	/*if (isInFuture((double)lhs.position_.X_, x, (double)lhs.velocity_.X_, 100.0)
		&& isInFuture((double)rhs.position_.X_, x, (double)rhs.velocity_.X_, 100.0))
	{
		return std::make_pair(x, y);
	}
	return {};*/
}

bool tryForGivenVX_VY(const InputType& input, long long VtrowX, long long VtrowY)
{
	// std::cout << "try: " << VtrowX << ", " << VtrowY << std::endl;

	Cube c0 = input[0];
	c0.velocity_.X_ -= VtrowX;
	c0.velocity_.Y_ -= VtrowY;

	double xPattern, yPattern;
	for (int i = 1; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.X_ -= VtrowX;
		c.velocity_.Y_ -= VtrowY;
		auto xyCurrent = calcCrossXY(c0, c);
		if (xyCurrent)
		{
			xPattern = xyCurrent->first;
			yPattern = xyCurrent->second;
			break;
		}
	}

	for (int i = 1; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.X_ -= VtrowX;
		c.velocity_.Y_ -= VtrowY;
		auto xyCurrent = calcCrossXY(c0, c);
		if (not xyCurrent)
		{
			continue;
		}
		if (not isEqual(xyCurrent->first, xPattern, 100.0) ||
			not isEqual(xyCurrent->second, yPattern, 100.0))
		{
			return false;
		}
	}
	std::cout << "Shall try: " << VtrowX << ", " << VtrowY << std::endl;
	std::cout << (long long)xPattern << ", " << (long long)yPattern << std::endl;
	return true;
}

long long Solution::solve_part2(const InputType& input) const
{
	Generator gVx;
	for (long long i = 0; i < 2000; ++i)
	{
		auto vx = gVx();
		Generator gVy;
		for (long long j = 0; j < 2000; ++j)
		{
			auto vy = gVy();
			if (tryForGivenVX_VY(input, vx, vy))
			{
				Generator gVz;
				for (long long k = 0; k < 2000; ++k)
				{
					auto vz = gVz();
					tryForGivenVX_VZ(input, vx, vz);
				}
			}

		}
	}
	return {};
}

}  // namespace year_2023::day24
