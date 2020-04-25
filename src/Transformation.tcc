#include "Math.h"
#include "Transformation.h"

namespace egl
{
template <typename T, std::size_t N>
Transformation<T, N>::Transformation(const Vec<T, N>& inPosition) : mPosition(inPosition)
{
}

template <typename T, std::size_t N>
Transformation<T, N>::Transformation(const Vec<T, N>& inPosition, const Transformation<T, N>::RotationType& inRotation)
    : mPosition(inPosition), mRotation(inRotation)
{
}

template <typename T, std::size_t N>
Transformation<T, N>::Transformation(const Vec<T, N>& inPosition,
    const Transformation<T, N>::RotationType& inRotation,
    const Vec<T, N>& inScale)
    : mPosition(inPosition), mRotation(inRotation), mScale(inScale)
{
}

template <typename T, std::size_t N>
Vec<T, N> Transformation<T, N>::Transformed(const Vec<T, N>& inPoint)
{
  return mPosition + (Rotated((inPoint * mScale), mRotation));
}

template <typename T, std::size_t N>
Vec<T, N> Transformation<T, N>::InverseTransformed(const Vec<T, N>& inPoint)
{
  return Rotated((inPoint - mPosition), -mRotation) / mScale;
}

template <typename T, std::size_t N>
Mat4f Transformation<T, N>::GetMatrix() const
{
  return TranslationMat(mPosition) * RotationMat(mRotation) * ScaleMat(mScale);
}

template <typename T, std::size_t N>
Mat4f Transformation<T, N>::GetInverseMatrix() const
{
  return ScaleMat(1.0f / mScale) * RotationMat(-mRotation) * TranslationMat(-mPosition);
}

}