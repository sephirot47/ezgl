#pragma once

#include "Span.h"
#include "Math.h"
#include <filesystem>
#include <vector>

namespace egl
{
template <typename T = Color4f>
class Image2D
{
public:
  Image2D();
  explicit Image2D(const std::filesystem::path &inImagePath);
  Image2D(const std::size_t inWidth, const std::size_t inHeight, const T &inFillValue);
  Image2D(const Image2D& inImage2D) = default;
  Image2D& operator=(const Image2D& inImage2D) = default;
  Image2D(Image2D&& inImage2D) = default;
  Image2D& operator=(Image2D&& inImage2D) = default;
  ~Image2D() = default;

  void Create(const std::size_t inWidth, const std::size_t inHeight);
  void Fill(const T &inValue);

  T& Get(const std::size_t inX, const std::size_t inY);
  const T& Get(const std::size_t inX, const std::size_t inY) const;

  void Read(const std::filesystem::path &inPath);
  void Write(const std::filesystem::path &inPath);

  Span<T> GetData() const;
  std::size_t GetWidth() const;
  std::size_t GetHeight() const;

private:
  std::vector<T> mData;
  std::size_t mWidth = 0;
  std::size_t mHeight = 0;
};
}

#include "Image2D.tcc"