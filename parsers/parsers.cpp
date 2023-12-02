#include "parsers.hpp"

namespace parsers
{

LinesInFileRange::LinesInFileRange(const char* fileLocation)
    : inputFile_(fileLocation)
{
}
LinesInFileRange::~LinesInFileRange()
{
    if (inputFile_.is_open())
    {
        inputFile_.close();
    }
}

bool LinesInFileRange::hasData() const
{
    return inputFile_ && not inputFile_.eof() && const_cast<std::fstream&>(inputFile_).peek() != EOF;
}

LinesInFileRange::TIterator::TIterator(LinesInFileRange& parent, bool endTag)
    : parent_(parent)
    , endTag_(endTag)
{
}

std::string& LinesInFileRange::TIterator::operator*()
{
    if (not parent_.line_)
    {
        std::string tmp;
        std::getline(parent_.inputFile_, tmp);
        parent_.line_ = std::move(tmp);
    }
    return *parent_.line_;
}

LinesInFileRange::TIterator LinesInFileRange::TIterator::operator++()
{
    parent_.line_ = {};
    if (not hasData())
    {
        endTag_ = true;
    }
    return *this;
}

bool LinesInFileRange::TIterator::hasData() const
{
    return parent_.hasData();
}

LinesInFileRange::TIterator LinesInFileRange::begin() noexcept { return TIterator(*this); }
LinesInFileRange::TIterator LinesInFileRange::end() noexcept { return TIterator(*this, true); }

std::vector <std::string> LinesInFileRange::toStringVector()
{
    std::vector<std::string> out;
    std::copy(begin(), end(), std::back_inserter(out));

    return out;
}

bool operator==(const LinesInFileRange::TIterator& lhs, const LinesInFileRange::TIterator& rhs)
{
    return not lhs.hasData();
}

bool operator!=(const LinesInFileRange::TIterator& lhs, const LinesInFileRange::TIterator& rhs)
{
    return lhs.hasData();
}

}  // namespace parsers
