#pragma once

#include "ez/Color.h"
#include "ez/MathInitializers.h"
#include "ez/Span.h"
#include <filesystem>
#include <vector>

namespace ez
{
template <typename T = Color4f>
class Image2D final
{
public:
  Image2D();
  explicit Image2D(const std::filesystem::path& inImagePath);
  Image2D(const std::size_t inWidth, const std::size_t inHeight, const T& inFillValue);
  Image2D(const Image2D& inImage2D) = default;
  Image2D& operator=(const Image2D& inImage2D) = default;
  Image2D(Image2D&& inImage2D) = default;
  Image2D& operator=(Image2D&& inImage2D) = default;
  ~Image2D() = default;

  void Create(const std::size_t inWidth, const std::size_t inHeight);
  void Fill(const T& inValue);

  T& Get(const std::size_t inX, const std::size_t inY);
  const T& Get(const std::size_t inX, const std::size_t inY) const;

  void Read(const std::filesystem::path& inPath);
  void Write(const std::filesystem::path& inPath) const;

  Span<T> GetData() const;
  std::size_t GetWidth() const;
  std::size_t GetHeight() const;

  std::vector<T>::iterator begin() { return mData.begin(); }
  std::vector<T>::iterator end() { return mData.end(); }
  std::vector<T>::const_iterator begin() const { return mData.begin(); }
  std::vector<T>::const_iterator end() const { return mData.end(); }
  std::vector<T>::const_iterator cbegin() const { return mData.cbegin(); }
  std::vector<T>::const_iterator cend() const { return mData.cend(); }

  bool operator==(const Image2D& inRHS) const;
  bool operator!=(const Image2D& inRHS) const;
  bool IsVeryEqual(const Image2D& inRHS, const T& inEpsilon = All<T>(static_cast<ValueType_t<T>>(1e-6))) const;

private:
  std::vector<T> mData;
  std::size_t mWidth = 0;
  std::size_t mHeight = 0;
};
}

#include "ez/Image2D.tcc"