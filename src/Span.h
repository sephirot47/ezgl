#pragma once

#include "Macros.h"
#include <array>
#include <cstdint>
#include <vector>

namespace egl
{
template <typename T>
struct Span
{
public:
  using ValueType = T;
  using ConstIterator = const T*;

  Span(const T* inData, const std::size_t inNumberOfElements) : mData(inData), mNumberOfElements(inNumberOfElements) {}

  ConstIterator begin() const { return mData; }
  ConstIterator end() const { return mData + mNumberOfElements; }
  ConstIterator cbegin() const { return begin(); }
  ConstIterator cend() const { return end(); }

  const T* GetData() const { return mData; }
  std::size_t GetNumberOfElements() const { return mNumberOfElements; }
  std::size_t GetSizeInBytes() const { return mNumberOfElements * sizeof(T); }

private:
  const T* mData = nullptr;
  const std::size_t mNumberOfElements = 0;
};

template <typename TArray>
inline Span<typename TArray::value_type> MakeSpan(const TArray& inArray)
{
  return Span<typename TArray::value_type> { inArray.data(), inArray.size() };
}
template <typename TArray>
inline Span<typename TArray::value_type> MakeSpan(const TArray& inArray, const std::size_t inNumberOfElements)
{
  EXPECTS(inNumberOfElements <= inArray.size());
  return Span<typename TArray::value_type> { inArray.data(), inNumberOfElements };
}
}