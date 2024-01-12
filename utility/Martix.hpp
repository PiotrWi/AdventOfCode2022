#pragma once

#include "PointRowCol.hpp"

#include <ranges>
#include <span>
#include <vector>
#include <utility>
#include <optional>
#include <type_traits>

template <typename TDataTypeIn, typename TRawMatrixTypeIn>
struct TypesTraits
{
	using TDataType = TDataTypeIn;
	using TRawMatrixType = TRawMatrixTypeIn;
	using TRowIterator = std::remove_reference<decltype(std::declval<TRawMatrixType>().begin())>::type;
	using TColumnIterator = std::remove_reference<decltype(std::declval<TRawMatrixType>()[0].begin())>::type;
	using TIteratorReturnValue = std::remove_reference<decltype(std::declval<TRawMatrixType>()[0][0])>::type;
	using TColumnType = std::remove_reference<decltype(std::declval<TRawMatrixType>()[0])>::type;
};

template <typename TDataTypeIn, typename TRawMatrixTypeIn>
class MatritWrapperIterator : public TypesTraits<TDataTypeIn, TRawMatrixTypeIn>
{
public:
	using Traits = TypesTraits<TDataTypeIn, TRawMatrixTypeIn>;

	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = typename Traits::TDataType;
	using pointer = Traits::TDataType*;
	using reference = Traits::TDataType&;
public:
	explicit MatritWrapperIterator(Traits::TRowIterator rowIterator)
		: rowIt_(rowIterator)
	{
	}

	MatritWrapperIterator& operator++()
	{
		if (not colIt_) colIt_ = rowIt_->begin();

		++(*colIt_);
		if (*colIt_ == rowIt_->end())
		{
			++rowIt_;
			colIt_ = {};
		}
		return *this;
	}

	Traits::TIteratorReturnValue& operator*()
	{
		if (not colIt_) colIt_ = rowIt_->begin();
		return (**colIt_);
	}

	pointer operator->()
	{
		return this->operator*();
	}

	bool operator==(const MatritWrapperIterator<TDataTypeIn, TRawMatrixTypeIn>& rhs)
	{
		return rowIt_ == rhs.rowIt_ && colIt_ == rhs.colIt_;
	}

	bool operator!=(const MatritWrapperIterator<TDataTypeIn, TRawMatrixTypeIn>& rhs)
	{
		return not (*this == rhs);
	}

	PointRowCol toPointRowCol(const MatritWrapperIterator<TDataTypeIn, TRawMatrixTypeIn>& begin)
	{
		auto thisCol = (colIt_) ? ((*colIt_) - rowIt_->begin()) : 0;
		return PointRowCol{ int(rowIt_ - begin.rowIt_), (int)thisCol };
	}
private:
	Traits::TRowIterator rowIt_;
	std::optional<typename Traits::TColumnIterator> colIt_;
};

template <typename TDataTypeIn, typename TRawMatrixTypeIn = std::vector<std::vector<TDataTypeIn>> >
class MatrixWrapper : public TypesTraits<TDataTypeIn, TRawMatrixTypeIn>
{
	using Traits = TypesTraits<TDataTypeIn, TRawMatrixTypeIn>;
	using TIteratorType = MatritWrapperIterator<TDataTypeIn, TRawMatrixTypeIn>;
	using TConstIteratorType = MatritWrapperIterator<const TDataTypeIn, const TRawMatrixTypeIn>;
public:
	explicit MatrixWrapper(TRawMatrixTypeIn& matrix)
		: matrix_(&matrix)
	{
	}

	explicit MatrixWrapper(int rows, int columns, TDataTypeIn&& initValue)
	{
		matrixData_ = std::vector<std::vector<TDataTypeIn>>(rows, std::vector<TDataTypeIn>(columns, initValue));
	}

	MatrixWrapper<TDataTypeIn, TRawMatrixTypeIn> operator=(MatrixWrapper&& other)
	{
		matrixData_ = std::move(other.matrixData_);
		if (matrixData_)
		{
			matrix_ = &(*matrixData_);
		}
		else
		{
			matrix_ = other.matrix_;
		}
		return *this;
	}

	MatrixWrapper(MatrixWrapper&& other)
	{
		matrixData_ = std::move(other.matrixData_);
		if (matrixData_)
		{
			matrix_ = &(*matrixData_);
		}
		else
		{
			matrix_ = other.matrix_;
		}
	}

	MatrixWrapper(const MatrixWrapper& other)
	{
		matrixData_ = other.matrixData_;
		if (matrixData_)
		{
			matrix_ = &(*matrixData_);
		}
		else
		{
			matrix_ = other.matrix_;
		}
	}

	MatrixWrapper operator=(const MatrixWrapper& other)
	{
		if (&other == this)
		{
			return *this;
		}
		matrixData_ = other.matrixData_;
		if (matrixData_)
		{
			matrix_ = &(*matrixData_);
		}
		else
		{
			matrix_ = other.matrix_;
		}
		return *this;
	}


	MatrixWrapper<TDataTypeIn, TRawMatrixTypeIn> cloneAndClear(TDataTypeIn&& initValue) const
	{
		MatrixWrapper<TDataTypeIn, TRawMatrixTypeIn> mv(this->getRowsCount(), this->getColumnsCount(), std::forward<TDataTypeIn>(initValue));
		return mv;
	}

	TConstIteratorType begin() const
	{
		return TConstIteratorType((*matrix_).begin());
	}

	TConstIteratorType end() const
	{
		return TConstIteratorType((*matrix_).end());
	}

	TIteratorType begin()
	{
		return TIteratorType((*matrix_).begin());
	}

	TIteratorType end()
	{
		return TIteratorType((*matrix_).end());
	}

	Traits::TColumnType & operator[](std::size_t rowIndex)
	{
		return (*matrix_)[rowIndex];
	}

	const Traits::TColumnType& operator[](std::size_t rowIndex) const
	{
		return (*matrix_)[rowIndex];
	}

	Traits::TDataType& operator[](const PointRowCol& point)
	{
		return (*matrix_)[point.row][point.col];
	}

	const Traits::TDataType& operator[](const PointRowCol& point) const
	{
		return (*matrix_)[point.row][point.col];
	}

	int getRowsCount() const
	{
		return (int)(*matrix_).size();
	}

	int getColumnsCount() const 
	{
		if (getRowsCount() == 0)
		{
			return 0;
		}
		return (int)(*matrix_)[0].size();
	}

	bool isInBound(const PointRowCol& point) const
	{
		return point.row >= 0 && point.col >= 0 && point.row < getRowsCount() and point.col < getColumnsCount();
	}
private:
	Traits::TRawMatrixType* matrix_;
	std::optional<typename Traits::TRawMatrixType> matrixData_;
};

template <typename TDataType>
auto createMatrixWrapper(const std::vector<std::vector<TDataType>>& in) -> const MatrixWrapper<TDataType, const std::vector< std::vector<TDataType> > >
{
	return MatrixWrapper<TDataType, const std::vector< std::vector<TDataType> > >(in);
}

template <typename TDataType>
auto createMatrixWrapper(std::vector<std::vector<TDataType>>& in)
{
	return MatrixWrapper<TDataType, std::vector<std::vector<TDataType>> >(in);
}

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

	explicit RowView(const std::span<TDataTypeIn>& data)
		: data_(data)
	{
	}

	size_type size() const
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

private:
	std::span<TDataTypeIn> data_;
};

template <typename TDataTypeIn>
class Matrix
{
public:
	using size_type = unsigned int;
	using row_view = RowView<TDataTypeIn>;
	using const_row_view = const RowView<const TDataTypeIn>;
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

	RowView<TDataTypeIn> operator[](size_type pos)
	{
		std::span<TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return RowView<TDataTypeIn>(row);
	}

	const RowView<const TDataTypeIn> operator[](size_type pos) const
	{
		std::span<const TDataTypeIn> row(data_.data() + pos * cols_, cols_);
		return RowView<const TDataTypeIn>(row);
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
	void push_back(TContainerType<TDataTypeIn, std::allocator<int>>&& row)
	{
		push_row(std::forward<TContainerType<TDataTypeIn, std::allocator<int>>&&>(row));
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
	void push_row(TContainerType<TDataTypeIn, std::allocator<int>>&& row)
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
