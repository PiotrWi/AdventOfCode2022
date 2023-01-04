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

	Traits::TIteratorReturnValue& operator->()
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
		auto thisCol = (colIt_) ? (**colIt_) : 0;
		return PointRowCol{ int(rowIt_ - begin.rowIt_), (int)thisCol };
	}
private:
	Traits::TRowIterator rowIt_;
	std::optional<typename Traits::TColumnIterator> colIt_;
};

template <typename TDataTypeIn, typename TRawMatrixTypeIn>
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
