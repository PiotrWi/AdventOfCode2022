#include "PointXYZ.hpp"

PointXYZ::PointXYZ(long long x, long long y, long long z)
	: x_(x)
	, y_(y)
	, z_(z)
{
}

bool operator< (const PointXYZ& lhs, const PointXYZ& rhs)
{
	return lhs.z_ < rhs.z_;
}

PointXYZ::PointXYZ(std::vector<long long> array)
	: PointXYZ(array[0], array[1], array[2])
{
}