#pragma once

#include <vector>
#include <utility>
#include <type_traits>

template <typename T>
auto toVector(T&& range)
{
	using TElemType = std::decay_t<decltype(*std::begin(range))>;
	std::vector <TElemType> out;
	for (auto&& elem : range)
	{
		out.emplace_back(elem);
	}
	return out;
}

struct ToVector {};

template <typename T>
auto operator|(const T& range, ToVector)
{
	return toVector(range);
}