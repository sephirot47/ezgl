#pragma once

#include "Mat.h"
#include "Quat.h"
#include "Vec.h"

namespace egl
{
// clang-format off
template <typename TTransformation> struct TransformationRotationType { using Type = void; };
// clang-format on

template <typename T, std::size_t N>
class Transformation
{
public:
  using ValueType = T;
  using RotationType = typename TransformationRotationType<Transformation>::Type;
  static constexpr auto Dimensions = N;

  Transformation() = default;
  explicit Transformation(const Vec<T, N>& inPosition);
  Transformation(const Vec<T, N>& inPosition, const RotationType& inRotation);
  Transformation(const Vec<T, N>& inPosition, const RotationType& inRotation, const Vec<T, N>& inScale);

  void SetPosition(const Vec<T, N>& inPosition) { mPosition = inPosition; }
  const Vec<T, N>& GetPosition() const { return mPosition; }

  void SetRotation(const RotationType& inRotation) { mRotation = inRotation; }
  const RotationType& GetRotation() const { return mRotation; }

  void SetScale(const Vec<T, N>& inScale) { mScale = inScale; }
  const Vec<T, N>& GetScale() const { return mScale; }

  Vec<T, N> Transformed(const Vec<T, N>& inPoint);
  Vec<T, N> InverseTransformed(const Vec<T, N>& inPoint);
  Mat4f GetMatrix() const;
  Mat4f GetInverseMatrix() const;

private:
  Vec<T, N> mPosition = Vec<T, N>(static_cast<T>(0.0f));
  RotationType mRotation = {}; // Identity
  Vec<T, N> mScale = Vec<T, N>(static_cast<T>(1.0f));
};

template <typename T>
using Transformation2 = Transformation<T, 2>;
using Transformation2f = Transformation2<float>;

template <typename T>
using Transformation3 = Transformation<T, 3>;
using Transformation3f = Transformation3<float>;

// clang-format off
template <typename T> struct TransformationRotationType<Transformation2<T>> { using Type = T; };
template <typename T> struct TransformationRotationType<Transformation3<T>> { using Type = Quat<T>; };
// clang-format on
}

#include "Transformation.tcc"