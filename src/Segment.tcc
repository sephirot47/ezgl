#include "Segment.h"

namespace egl
{
template <typename T, std::size_t N>
Segment<T, N>::Segment(const Vec<T, N>& inFromPoint, const Vec<T, N>& inToPoint)
    : mFromPoint(inFromPoint), mToPoint(inToPoint)
{
}

template <typename T, std::size_t N>
Vec<T, N> Segment<T, N>::GetFromPoint() const
{
  return mFromPoint;
}

template <typename T, std::size_t N>
Vec<T, N> Segment<T, N>::GetToPoint() const
{
  return mToPoint;
}

template <typename T, std::size_t N>
Vec<T, N> Segment<T, N>::GetVector() const
{
  return (mToPoint - mFromPoint);
}
}