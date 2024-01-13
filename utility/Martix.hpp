#pragma once

#include "PointRowCol.hpp"

#include <ranges>
#include <span>
#include <vector>
#include <utility>
#include <optional>
#include <type_traits>

template <typename TDataTypeIn>
class MatrixIterator
{
public:
	using value_type = TDataTypeIn;
	using reference = TDataTypeIn&;
	using pointer = TDataTypeIn*;
	using difference_type = unsigned int;
	using iterator_category = std::random_access_iterator_tag;

	explicit MatrixIterator(TDataTypeIn* begin, TDataTypeIn* curent, unsigned int cols) noexcept
		: begin_(begin)
		, curent_(curent)
		, cols_(cols)
	{
	}

	MatrixIterator& operator++() noexcept
	{
		++curent_;
		return *this;
	}

	MatrixIterator& operator++(int) noexcept
	{
		auto other = *this;
		++curent_;
		return other;
	}

	MatrixIterator& operator--() noexcept
	{
		--curent_;
		return *this;
	}

	MatrixIterator& operator--(int) noexcept
	{
		auto other = *this;
		--curent_;
		return other;
	}

	bool operator!=(const MatrixIterator& rhs) const noexcept
	{
		return this->curent_ != rhs.curent_;
	}

	bool operator==(const MatrixIterator& rhs) const noexcept
	{
		return this->curent_ == rhs.curent_;
	}

	bool operator < (const MatrixIterator& rhs) const noexcept
	{
		return this->curent_ < rhs.curent_;
	}

	MatrixIterator& operator += (difference_type n)  noexcept
	{
		curent_ += n;
		return *this;
	}

	MatrixIterator operator+ (difference_type n) const noexcept
	{
		MatrixIterator other = *this;
		other.curent_ += n;
		return other;
	}

	reference operator[](difference_type n) const noexcept
	{
		MatrixIterator other = *this;
		other.curent_ += n;
		return *other;
	}

	difference_type operator-(const MatrixIterator& other) const noexcept
	{
		return curent_ - other.curent_;
	}

	PointRowCol getPoint() const noexcept
	{
		PointRowCol point;
		point.row = getRow();
		point.col = getCol();
		return point;
	}

	difference_type getRow() const noexcept
	{
		auto distance = curent_ - begin_;
		return distance / cols_;
	}

	difference_type getCol() const noexcept
	{
		auto distance = curent_ - begin_;
		return distance % cols_;
	}

	reference operator*() noexcept
	{
		return (*curent_);
	}

	pointer operator->() noexcept
	{
		return curent_;
	}
private:
	TDataTypeIn* begin_;
	TDataTypeIn* curent_;
	unsigned int cols_;
};


template <typename TDataTypeIn>
class RowView
{
public:
	using reference = TDataTypeIn&;
	using const_reference = const TDataTypeIn&;

	using size_type = unsigned int;

	using iterator = typename std::span<TDataTypeIn>::iterator;

	explicit RowView(const std::span<TDataTypeIn>& data)
		: data_(data)
	{
	}

	size_type size() const noexcept
	{
		return data_.size();
	}

	reference operator[](size_type pos)
	{
		return data_[pos];
	}

	const_reference operator[](size_type pos) const
	{
		return data_[pos];
	}

	iterator begin() noexcept
	{
		return data_.begin();
	}

	iterator end() noexcept
	{
		return data_.end();
	}

	iterator begin() const noexcept
	{
		return data_.begin();
	}

	iterator end() const noexcept
	{
		return data_.end();
	}
private:
	std::span<TDataTypeIn> data_;
};

template <typename TDataTypeIn> class Matrix;

template <typename TDataTypeIn>
class RowsRange
{
public:
	explicit RowsRange(Matrix<TDataTypeIn>& matrix)
		: matrix_(matrix)
	{
		for (auto i = 0u; i < matrix.rows_count(); ++i)
		{
			rows_.push_back(matrix.row(i));
		}
	}

	std::vector<RowView<TDataTypeIn>>::iterator begin()
	{
		return rows_.begin();
	}

	std::vector<RowView<TDataTypeIn>>::iterator end()
	{
		return rows_.end();
	}

private:
	Matrix<TDataTypeIn>& matrix_;
	std::vector<RowView<TDataTypeIn>> rows_;
};

template <typename TDataTypeIn>
class Matrix
{
public:
	using size_type = unsigned int;
	using row_view = RowView<TDataTypeIn>;
	using const_row_view = const RowView<const TDataTypeIn>;
	using iterator = MatrixIterator<TDataTypeIn>;
	using const_iterator = MatrixIterator<const TDataTypeIn>;
	using reference = TDataTypeIn&;
	using const_reference = const TDataTypeIn&;

	explicit Matrix()
	{
	}

	explicit Matrix(unsigned int cols)
		: cols_(cols)
	{
	}

	explicit Matrix(unsigned int rows, unsigned int cols, TDataTypeIn initialValue)
		: rows_(rows)
		, cols_(cols)
		, data_(rows_* cols_, initialValue)
	{
	}

	size_type rows_count() const
	{
		return rows_;
	}

	size_type cols_count() const
	{
		return cols_;
	}

	bool inBounds(const PointRowCol point) const
	{
		return point.row >= 0 && point.row < rows_ && point.col >= 0 && point.col < cols_;
	}

	row_view operator[](size_type pos)
	{
		std::span<TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return row_view(row);
	}

	const RowView<const TDataTypeIn> operator[](size_type pos) const
	{
		std::span<const TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return RowView<const TDataTypeIn>(row);
	}

	RowsRange<TDataTypeIn> rows()
	{
		return RowsRange<TDataTypeIn>(*this);
	}

	RowView<TDataTypeIn> row(size_type pos)
	{
		std::span<TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return RowView<TDataTypeIn>(row);
	}

	const RowView<const TDataTypeIn> row(size_type pos) const
	{
		std::span<const TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return RowView<const TDataTypeIn>(row);
	}

	reference operator[](const PointRowCol point)
	{
		return *(data_.data() + point.row * cols_ + point.col);
	}

	const_reference operator[](const PointRowCol point) const
	{
		return *(data_.data() + point.row * cols_ + point.col);
	}

	iterator begin() noexcept
	{
		return MatrixIterator(data_.data(), data_.data(), cols_);
	}

	iterator end() noexcept
	{
		return MatrixIterator(data_.data(), data_.data() + data_.size(), cols_);
	}

	const_iterator begin() const noexcept
	{
		return MatrixIterator(data_.data(), data_.data(), cols_);
	}

	const_iterator end() const noexcept
	{
		return MatrixIterator(data_.data(), data_.data() + data_.size(), cols_);
	}

	const_reference back() const
	{
		return data_.back();
	}

	template <template<typename, typename> class TContainerType>
	void push_back(TContainerType<TDataTypeIn, std::allocator<TDataTypeIn>>&& row)
	{
		push_row(std::forward<TContainerType<TDataTypeIn, std::allocator<TDataTypeIn>>&&>(row));
	}

	template <template<typename> class TContainerType>
	void emplace_back(TContainerType<TDataTypeIn>&& row)
	{
		push_row(std::forward(row));
	}

	void emplace_back(const std::string& row)
	{
		++rows_;
		std::ranges::copy(row, std::back_inserter(data_));
		if (cols_ == 0)
		{
			cols_ = row.size();
		}
	}

	template <template<typename, typename> class TContainerType>
	void push_row(TContainerType<TDataTypeIn, std::allocator<TDataTypeIn>>&& row)
	{
		++rows_;
		std::ranges::copy(row, std::back_inserter(data_));
		if (cols_ == 0)
		{
			cols_ = row.size();
		}
	}

private:
	size_type rows_ = 0;
	size_type cols_ = 0;
	std::vector<TDataTypeIn> data_;
};

template <typename TDataTypeIn>
auto getInBoundsNeighbours(const Matrix<TDataTypeIn>& matrix, PointRowCol point)
{
	std::vector<PointRowCol> out;
	for (auto diff : { BottomPointDiff, UpperPointDiff, LeftPointDiff, RightPointDiff })
	{
		auto neighbour = point + diff;
		if (matrix.inBounds(neighbour))
		{
			out.push_back(neighbour);
		}
	}
	return out;
}
