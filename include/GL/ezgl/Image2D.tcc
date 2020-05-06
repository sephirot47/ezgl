#include "ezgl/Image2D.h"
#include "ezgl/ImageIO.h"
#include "ezgl/Math.h"

namespace ez
{

template <typename T>
Image2D<T>::Image2D()
{
  Create(1, 1);
}

template <typename T>
Image2D<T>::Image2D(const std::filesystem::path& inImagePath)
{
  Read(inImagePath);
}

template <typename T>
Image2D<T>::Image2D(const std::size_t inWidth, const std::size_t inHeight, const T& inFillValue)
{
  Create(inWidth, inHeight);
  Fill(inFillValue);
}

template <typename T>
void Image2D<T>::Create(const std::size_t inWidth, const std::size_t inHeight)
{
  mData = std::vector<T>(inWidth * inHeight);
  mWidth = inWidth;
  mHeight = inHeight;
}

template <typename T>
void Image2D<T>::Fill(const T& inValue)
{
  std::fill(mData.begin(), mData.end(), inValue);
}

template <typename T>
T& Image2D<T>::Get(const std::size_t inX, const std::size_t inY)
{
  return mData.at(inX + inY * mWidth);
}

template <typename T>
const T& Image2D<T>::Get(const std::size_t inX, const std::size_t inY) const
{
  return const_cast<Image2D<T>&>(*this).Get(inX, inY);
}

template <typename T>
void Image2D<T>::Read(const std::filesystem::path& inPath)
{
  ImageIO::Read(inPath, *this);
}

template <typename T>
void Image2D<T>::Write(const std::filesystem::path& inPath) const
{
  ImageIO::Write(*this, inPath);
}

template <typename T>
Span<T> Image2D<T>::GetData() const
{
  return MakeSpan(mData, mWidth * mHeight);
}

template <typename T>
std::size_t Image2D<T>::GetWidth() const
{
  return mWidth;
}

template <typename T>
std::size_t Image2D<T>::GetHeight() const
{
  return mHeight;
}

template <typename T>
bool Image2D<T>::operator==(const Image2D& inRHS) const
{
  return mData == inRHS.mData && mWidth == inRHS.mWidth && mHeight == inRHS.mHeight;
}

template <typename T>
bool Image2D<T>::operator!=(const Image2D& inRHS) const
{
  return !(*this == inRHS);
}

template <typename T>
bool Image2D<T>::IsVeryEqual(const Image2D& inRHS, const T& inEpsilon) const
{
  if (mWidth != inRHS.mWidth)
    return false;

  if (mHeight != inRHS.mHeight)
    return false;

  auto it = cbegin();
  auto rhs_it = inRHS.cbegin();
  while (it != cend())
  {
    const auto& pixel = *it;
    const auto& rhs_pixel = *rhs_it;

    if (!::ez::IsVeryEqual(pixel, rhs_pixel, inEpsilon))
      return false;

    ++it;
    ++rhs_it;
  }
  assert(it == cend());
  assert(rhs_it == inRHS.cend());
  return true;
}
}