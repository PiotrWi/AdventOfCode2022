#pragma once

#include "PointRowCol.hpp"

#include <vector>
#include <utility>
#include <optional>
#include <type_traits>

template <typename TDataTypeIn, typename TRawMatrixTypeIn>
struct TypesTraits
{
	using TDataType = typename TDataTypeIn;
	using TRawMatrixType = typename TRawMatrixTypeIn;
	using TRowIterator = typename std::remove_reference<decltype(std::declval<TRawMatrixType>().begin())>::type;
	using TColumnIterator = typename std::remove_reference<decltype(std::declval<TRawMatrixType>()[0].begin())>::type;
	using TIteratorReturnValue = typename std::remove_reference<decltype(std::declval<TRawMatrixType>()[0][0])>::type;
	using TColumnType = typename std::remove_reference<decltype(std::declval<TRawMatrixType>()[0])>::type;
};

template <typename TDataTypeIn, typename TRawMatrixTypeIn>
class MatritWrapperIterator : public TypesTraits<TDataTypeIn, TRawMatrixTypeIn>
{
public:
	using Traits = typename TypesTraits<TDataTypeIn, TRawMatrixTypeIn>;

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
	using Traits = typename TypesTraits<TDataTypeIn, TRawMatrixTypeIn>;
	using TIteratorType = MatritWrapperIterator<TDataTypeIn, TRawMatrixTypeIn>;
	using TConstIteratorType = MatritWrapperIterator<const TDataTypeIn, const TRawMatrixTypeIn>;
public:
	explicit MatrixWrapper(TRawMatrixTypeIn& matrix)
		: matrix_(matrix)
	{
	}

	TConstIteratorType begin() const
	{
		return TConstIteratorType(matrix_.begin());
	}

	TConstIteratorType end() const
	{
		return TConstIteratorType(matrix_.end());
	}

	TIteratorType begin()
	{
		return TIteratorType(matrix_.begin());
	}

	TIteratorType end()
	{
		return TIteratorType(matrix_.end());
	}

	Traits::TColumnType & operator[](std::size_t rowIndex)
	{
		return matrix_[rowIndex];
	}

	Traits::TDataType& operator[](const PointRowCol& point)
	{
		return matrix_[point.row][point.col];
	}

	const Traits::TDataType& operator[](const PointRowCol& point) const
	{
		return matrix_[point.row][point.col];
	}

	int getRowsCount()
	{
		return (int)matrix_.size();
	}

	int getColumnsCount()
	{
		if (getRowsCount() == 0)
		{
			return 0;
		}
		return (int)matrix_[0].size();
	}

	bool isInBound(const PointRowCol& point)
	{
		return point.row >= 0 && point.col >= 0 && point.row < getRowsCount() and point.col < getColumnsCount();
	}
private:
	Traits::TRawMatrixType& matrix_;
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
