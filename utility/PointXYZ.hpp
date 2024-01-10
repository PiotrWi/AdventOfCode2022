#pragma once

#include <vector>

struct PointXYZ
{
	PointXYZ() = default;
	explicit PointXYZ(long long x, long long y, long long z);
	explicit PointXYZ(std::vector<long long> array);
	long long x_;
	long long y_;
	long long z_;
};

bool operator< (const PointXYZ& lhs, const PointXYZ& rhs);
