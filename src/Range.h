#pragma once

namespace egl
{

template <typename TIterator>
class Range
{
public:
    Range(TIterator inBegin, TIterator inEnd)
        : mBegin(inBegin)
        , mEnd(inEnd)
    {
    }

    TIterator begin() { return mBegin; }
    TIterator end() { return mEnd; }
    TIterator cbegin() const { return mBegin; }
    TIterator cend() const { return mEnd; }

private:
    const TIterator mBegin;
    const TIterator mEnd;
};

}