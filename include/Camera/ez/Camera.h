#pragma once

#include <ez/MathInitializers.h>
#include <ez/Transformation.h>
#include <ez/Vec.h>
#include <cstdint>

namespace ez
{
template <typename T, std::size_t N>
class Camera
{
public:
  using RotationType = typename Transformation<T, N>::RotationType;

  Camera() = default;
  Camera(const Camera& inRHS) = default;
  Camera& operator=(const Camera& inRHS) = default;
  Camera(Camera&& ioRHS) = default;
  Camera& operator=(Camera&& ioRHS) = default;
  virtual ~Camera() = default;

  void SetPosition(const Vec<T, N>& inPosition) { mTransformation.SetPosition(inPosition); }
  const Vec<T, N>& GetPosition() const { return mTransformation.GetPosition(); }

  void SetRotation(const Camera::RotationType& inRotation);
  const Camera::RotationType& GetRotation() const { return mTransformation.GetRotation(); }

  void LookAtPoint(const Vec<T, N>& inPointToLookAt, const Vec<T, N>& inUpNormalized = Up<Vec<T, N>>());

  Vec<T, N> GetForward() const { return GetRotation() * Forward<Vec<T, N>>(); }
  Vec<T, N> GetRight() const { return GetRotation() * Right<Vec<T, N>>(); }
  Vec<T, N> GetUp() const { return GetRotation() * Up<Vec<T, N>>(); }

  const Transformation<T, N>& GetTransformation() const { return mTransformation; }
  SquareMat<T, N + 1> GetModelMatrix() const { return mTransformation.GetMatrix(); }
  SquareMat<T, N + 1> GetViewMatrix() const { return mTransformation.GetInverseMatrix(); }
  virtual SquareMat<T, N + 1> GetProjectionMatrix() const = 0;

private:
  Transformation<T, N> mTransformation;
};

template <typename T>
using Camera2 = Camera<T, 2>;
using Camera2f = Camera2<float>;

template <typename T>
using Camera3 = Camera<T, 3>;
using Camera3f = Camera3<float>;
}

#include "ez/Camera.tcc"