#include "Solution.hpp"

#include <array>
#include <queue>

#include <parsers/parsers.hpp>
#include <utility/PointRowColOrientation.hpp>
#include <utility/RangesUtils.hpp>

namespace year_2023::day16
{

namespace
{

void visit(std::vector<std::vector < std::array<bool, 4> > >& visited, const InputType& input, PointRowColOrientation point)
{
	std::queue<PointRowColOrientation> toVisit;
	toVisit.push(point);

	while (not toVisit.empty())
	{
		auto point = toVisit.front();
		toVisit.pop();

		auto p = point.position_;
		if (not inBounds(point.position_, input.size(), input[0].size()) || visited[p.row][p.col][static_cast<int>(point.orientation_)] == true)
		{
			continue;
		}
		visited[p.row][p.col][static_cast<int>(point.orientation_)] = true;

		switch (input[p.row][p.col])
		{
		case '.':
			point.position_ = point.position_ + getDirrectionDif(point.orientation_);
			toVisit.push(point);
			continue;
		case '\\':
			if (point.orientation_ == Dir::right || point.orientation_ == Dir::left)
			{
				point.orientation_ = rotateRight(point.orientation_);
			}
			else
			{
				point.orientation_ = rotateLeft(point.orientation_);
			}
			point.position_ = point.position_ + getDirrectionDif(point.orientation_);
			toVisit.push(point);
			continue;
		case '/':
			if (point.orientation_ == Dir::right || point.orientation_ == Dir::left)
			{
				point.orientation_ = rotateLeft(point.orientation_);
			}
			else
			{
				point.orientation_ = rotateRight(point.orientation_);
			}
			point.position_ = point.position_ + getDirrectionDif(point.orientation_);
			toVisit.push(point);
			continue;
		case '|':
			if (point.orientation_ == Dir::right || point.orientation_ == Dir::left)
			{
				auto upperPoint = point;
				upperPoint.orientation_ = Dir::up;
				upperPoint.position_ = upperPoint.position_ + getDirrectionDif(upperPoint.orientation_);
				toVisit.push(upperPoint);

				auto downPoint = point;
				downPoint.orientation_ = Dir::down;
				downPoint.position_ = downPoint.position_ + getDirrectionDif(downPoint.orientation_);
				toVisit.push(downPoint);
			}
			else
			{
				point.position_ = point.position_ + getDirrectionDif(point.orientation_);
				toVisit.push(point);
			}
			continue;
		case '-':
			if (point.orientation_ == Dir::up || point.orientation_ == Dir::down)
			{
				auto leftPoint = point;
				leftPoint.orientation_ = Dir::left;
				leftPoint.position_ = leftPoint.position_ + getDirrectionDif(leftPoint.orientation_);
				toVisit.push(leftPoint);

				auto rightPoint = point;
				rightPoint.orientation_ = Dir::right;
				rightPoint.position_ = rightPoint.position_ + getDirrectionDif(rightPoint.orientation_);
				toVisit.push(rightPoint);
			}
			else
			{
				point.position_ = point.position_ + getDirrectionDif(point.orientation_);
				toVisit.push(point);
			}
			continue;
		}
	}
}

long long collectEnergizedPoints(const std::vector<std::vector < std::array<bool, 4> > >& visitedNodes)
{
	auto sum = 0ull;
	for (auto row = 0; row < visitedNodes.size(); ++row)
	{
		for (auto col = 0; col < visitedNodes[row].size(); ++col)
		{
			sum += visitedNodes[row][col][0] || visitedNodes[row][col][1] || visitedNodes[row][col][2] || visitedNodes[row][col][3];
		}
	}
	return sum;
}

long long getEnergizedPoints(const InputType& input, PointRowColOrientation point)
{
	std::vector<std::vector < std::array<bool, 4> > > visitedNodes(input.size(), std::vector<std::array<bool, 4> >(input[0].size(), { false, false, false, false }));

	visit(visitedNodes, input, point);
	return collectEnergizedPoints(visitedNodes);
}

}  // namespace


InputType parse()
{
	return parsers::getFile(2023, 16) | To<InputType>();
}

long long Solution::solve(const InputType& input) const
{
	return getEnergizedPoints(input, { Dir::right, {0, 0} });
}

long long Solution::solve_part2(const InputType& input) const
{
	auto max = 0ll;
	for (auto row = 0; row < input.size(); ++row)
	{
		max = std::max(max, getEnergizedPoints(input, { Dir::right, {row, 0} }));
		max = std::max(max, getEnergizedPoints(input, { Dir::left, {row, int(input[row].size() - 1)}}));
	}

	for (auto col = 0; col < input[0].size(); ++col)
	{
		max = std::max(max, getEnergizedPoints(input, { Dir::down, {0, col} }));
		max = std::max(max, getEnergizedPoints(input, { Dir::up, {int(input.size() - 1), col} }));
	}

	return max;
}

}  // namespace year_2023::day16
