#include "Solution.hpp"

#include <ranges>
#include <iostream>
#include <map>
#include <variant>
#include <utility/RangesUtils.hpp>
#include <iomanip>
#include <limits>

#include "parsers/parsers.hpp"
#include "StringAlgorithms/StringAlgorithms.hpp"
#include <boost/multiprecision/cpp_int.hpp>

namespace year_2023::day24
{

using namespace boost::multiprecision;

InputType parse()
{
	InputType in;
	for (auto&& line : parsers::getFile(2023, 24))
	{
		Cube c;
		auto positionToVelocity = splitAndTrim(line, '@');
        auto positionsStrs = splitAndTrim(positionToVelocity[0], ',');
		auto positions = positionsStrs | std::views::transform([](auto&& str) { return std::stod(str); }) | To<std::vector<long long>>();
		c.position_ = PointXYZ(positions);

        auto velocitiesStrs = splitAndTrim(positionToVelocity[1], ',');
		auto velocites = velocitiesStrs | std::views::transform([](auto&& str) { return std::stod(str); }) | To<std::vector<long long>>();
		c.velocity_ = PointXYZ(velocites);

		in.push_back(c);
	}

	return in;
}

template <long long RangeBegin, long long RangeEnd>
bool inRange(cpp_rational val)
{
	return (val >= RangeBegin) && (val <= RangeEnd);
}

bool isInFuture(cpp_rational val_0, cpp_rational val, cpp_rational vel)
{
	if (vel > 0)
	{
		return val > (val_0);
	}
	return val < val_0;
}

template <long long RangeBegin, long long RangeEnd>
bool match(Cube lhs, Cube rhs)
{
	auto a_lhs = (cpp_rational)lhs.velocity_.y_ / lhs.velocity_.x_;
	auto b_lhs = (cpp_rational)lhs.position_.y_ - a_lhs * lhs.position_.x_;

	auto a_rhs = (cpp_rational)rhs.velocity_.y_ / rhs.velocity_.x_;
	auto b_rhs = (cpp_rational)rhs.position_.y_ - a_rhs * rhs.position_.y_;

	if (a_lhs == a_rhs)
	{
		return b_lhs == b_rhs;
	}

	auto x = (b_rhs - b_lhs) / (a_lhs - a_rhs);
	auto y = a_lhs * x + b_lhs;

	return inRange<RangeBegin, RangeEnd>(x)
		&& inRange<RangeBegin, RangeEnd>(y)
		&& isInFuture((double)lhs.position_.x_, x, (double)lhs.velocity_.x_)
		&& isInFuture((double)rhs.position_.x_, x, (double)rhs.velocity_.x_);
}

long long Solution::solve(const InputType& input) const
{
	auto count = 0ll;
	for (auto lhs = input.begin(); lhs != input.end(); ++lhs)
	{
		for (auto rhs = lhs + 1; rhs != input.end(); ++rhs)
		{
			// count += match<7.0, 24.0>(*lhs, *rhs);  // test input
			count += match < 200000000000000ll, 400000000000000ll > (*lhs, *rhs);
		}
	}
	return count;
}

struct Generator
{
	long long operator()()
	{
		sign_ *= -1;
		++value_;
		return (value_ / 2) * sign_;
	}
	long long sign_ = -1;
	long long value_ = 1;
};

struct AnyPoint {};
struct NonePoint {};
using RationalPoint = std::pair<cpp_rational, cpp_rational>;

using CrossResult = std::variant<RationalPoint, AnyPoint, NonePoint>;

template <long long PointXYZ::*TYAxisPtr>
CrossResult calcCross(Cube lhs, Cube rhs)
{
	if (lhs.velocity_.x_ == 0 && rhs.velocity_.x_ == 0)
	{
		if (lhs.position_.x_ != rhs.position_.x_)
		{
			return NonePoint{};
		}
	}
	if (lhs.velocity_.x_ == 0 || rhs.velocity_.x_ == 0)
	{
		return AnyPoint{};
	}
	auto& lhs_velocity_y = lhs.velocity_.*TYAxisPtr;
	auto& lhs_position_y = lhs.position_.*TYAxisPtr;
	auto& rhs_velocity_y = rhs.velocity_.*TYAxisPtr;
	auto& rhs_position_y = rhs.position_.*TYAxisPtr;

	auto a_lhs = (cpp_rational)lhs_velocity_y / lhs.velocity_.x_;
	auto b_lhs = (cpp_rational)lhs_position_y - a_lhs * lhs.position_.x_;

	auto a_rhs = (cpp_rational)rhs_velocity_y / rhs.velocity_.x_;
	auto b_rhs = (cpp_rational)rhs_position_y - a_rhs * rhs.position_.x_;

	if (a_lhs == a_rhs)
	{
		if (b_rhs == b_rhs)
		{
			return AnyPoint{};
		}
		return NonePoint{};
	}

	auto x = (b_rhs - b_lhs) / (a_lhs - a_rhs);
	auto z = a_lhs * x + b_lhs;
	return std::make_pair(x, z);
}

template <long long PointXYZ::*TYAxisPtr>
std::optional<RationalPoint> findFirstConcreteCrossPoint(const InputType& input, Cube lhs, long long VtrowX, long long VtrowY)
{
	std::optional<RationalPoint> pattern;
	for (auto i = 1u; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.x_ -= VtrowX;
		c.velocity_.*TYAxisPtr -= VtrowY;
		auto Current = calcCross<TYAxisPtr>(lhs, c);
		if (const auto* point = std::get_if<RationalPoint>(&Current))
		{
			return *point;
		}
	}
	return pattern;
}

template <long long PointXYZ::*TYAxisPtr>
std::optional<RationalPoint> tryForGivenVxVy(const InputType& input, long long VtrowX, long long VtrowY)
{
	Cube c0 = input[0];
	c0.velocity_.x_ -= VtrowX;
	c0.velocity_.*TYAxisPtr -= VtrowY;

	auto pattern = findFirstConcreteCrossPoint<TYAxisPtr>(input, c0, VtrowX, VtrowY);
	if (not pattern)
	{
		return {};
	}

	for (auto i = 1u; i < input.size(); ++i)
	{
		auto c = input[i];
		c.velocity_.x_ -= VtrowX;
		c.velocity_.*TYAxisPtr -= VtrowY;
		auto xyCurrent = calcCross<TYAxisPtr>(c0, c);
		if (std::get_if<NonePoint>(&xyCurrent))
		{
			return {};
		}
		if (std::get_if<AnyPoint>(&xyCurrent))
		{
			continue;
		}
		if (std::get<RationalPoint>(xyCurrent) != pattern)
		{
			return {};
		}
	}
	return *pattern;
}

long long Solution::solve_part2(const InputType& input) const
{
	Generator gVx;
	for (long long i = 0; i < 700; ++i)
	{
		auto vx = gVx();
		Generator gVy;
		for (long long j = 0; j < 700; ++j)
		{
			auto vy = gVy();
			auto pointXY = tryForGivenVxVy<&PointXYZ::y_>(input, vx, vy);
			if (pointXY)
			{
				Generator gVz;
				for (long long k = 0; k < 700; ++k)
				{
					auto vz = gVz();
					auto pointXZ = tryForGivenVxVy<&PointXYZ::z_>(input, vx, vz);
					if (pointXZ)
					{
						return boost::multiprecision::numerator(pointXY->first).convert_to<long long>()
							+ boost::multiprecision::numerator(pointXY->second).convert_to<long long>()
							+ boost::multiprecision::numerator(pointXZ->second).convert_to<long long>();
					}
				}
			}
		}
	}

	return {};
}

}  // namespace year_2023::day24
