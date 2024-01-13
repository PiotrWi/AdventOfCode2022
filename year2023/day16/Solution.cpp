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

void visit(Matrix<std::array<bool, 4>>& visited, const InputType& input, PointRowColOrientation point)
{
	std::queue<PointRowColOrientation> toVisit;
	toVisit.push(point);

	while (not toVisit.empty())
	{
		auto point = toVisit.front();
		toVisit.pop();

		auto p = point.position_;
		if (not input.inBounds(point.position_) || visited[p][static_cast<int>(point.orientation_)] == true)
		{
			continue;
		}
		visited[p][static_cast<int>(point.orientation_)] = true;

		switch (input[p])
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

long long collectEnergizedPoints(Matrix<std::array<bool, 4>>& visitedNodes)
{
	return std::count_if(visitedNodes.begin(), visitedNodes.end(), [](auto&& visitedTable){
		return std::ranges::any_of(visitedTable, [](auto visited) { return visited; });
	});
}

long long getEnergizedPoints(const InputType& input, PointRowColOrientation point)
{
	Matrix<std::array<bool, 4>> visitedNodes(input.rows_count(), input.cols_count(), { false, false, false, false });
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
	for (auto row = 0u; row < input.rows_count(); ++row)
	{
		max = std::max(max, getEnergizedPoints(input, { Dir::right, {(int)row, 0} }));
		max = std::max(max, getEnergizedPoints(input, { Dir::left, {(int)row, int(input.cols_count() - 1)}}));
	}

	for (auto col = 0u; col < input.cols_count(); ++col)
	{
		max = std::max(max, getEnergizedPoints(input, { Dir::down, {0, (int)col} }));
		max = std::max(max, getEnergizedPoints(input, { Dir::up, {int(input.rows_count() - 1), (int)col} }));
	}

	return max;
}

}  // namespace year_2023::day16
