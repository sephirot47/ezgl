#include "Image2D.h"
#include "ImageIO.h"

namespace egl
{

template <typename T>
Image2D<T>::Image2D()
{
  Create(1, 1);
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
void Image2D<T>::Write(const std::filesystem::path& inPath)
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

}