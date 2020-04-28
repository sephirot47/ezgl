#pragma once

#include "Color.h"
#include "Mat.h"
#include "Quat.h"
#include "Segment.h"
#include "TypeTraits.h"
#include "Vec.h"
#include <cmath>
#include <cstdint>
#include <tuple>

#include "MathInitializers.h"

namespace egl
{
template <typename T>
constexpr T ColorCast(const Color4f& inColor)
{
  if constexpr (T::NumComponents == 3)
    return T { inColor[0], inColor[1], inColor[2] };
  else
    return T { inColor[0], inColor[1], inColor[2], inColor[3] };
}
template <typename T>
constexpr auto Abs(const T& inValue)
{
  if constexpr (IsNumber_v<T>)
  {
    return std::abs(inValue);
  }
  else
  {
    auto result = inValue;
    for (auto& v : result) { v = Abs(v); }
    return result;
  }
}

template <typename T>
constexpr T Clamp(const T& inValue, const T& inMin, const T& inMax)
{
  if (inValue < inMin)
    return inMin;
  if (inValue > inMax)
    return inMax;
  return inValue;
}

template <typename T>
constexpr T Sign(const T& inValue)
{
  return (inValue < 0 ? static_cast<T>(-1) : static_cast<T>(1));
}
template <typename T>
constexpr auto Dot(const T& inLHS, const T& inRHS)
{
  using ValueType = typename T::ValueType;

  auto dot = static_cast<ValueType>(0);
  for (std::size_t i = 0; i < T::NumComponents; ++i) { dot += inLHS[i] * inRHS[i]; }
  return dot;
}

template <typename T>
constexpr Vec3<T> Cross(const Vec3<T>& inLHS, const Vec3<T>& inRHS)
{
  return Vec3<T> { inLHS[1] * inRHS[2] - inLHS[2] * inRHS[1],
    inLHS[2] * inRHS[0] - inLHS[0] * inRHS[2],
    inLHS[0] * inRHS[1] - inLHS[1] * inRHS[0] };
}

template <typename T>
constexpr auto SqLength(const T& inV)
{
  return Dot(inV, inV);
}

template <typename T>
constexpr auto Length(const T& inV)
{
  return std::sqrt(SqLength(inV));
}
template <typename T>
constexpr auto SqDistance(const T& inLHS, const T& inRHS)
{
  const auto diff = (inRHS - inLHS);
  return SqLength(diff, diff);
}

template <typename T>
constexpr auto Distance(const T& inLHS, const T& inRHS)
{
  if constexpr (IsNumber_v<T>)
  {
    return Abs(inLHS - inRHS);
  }
  else
  {
    return std::sqrt(SqDistance(inLHS, inRHS));
  }
}

template <typename T>
constexpr bool VeryEqual(const T& inLHS, const T& inRHS, const T& inEpsilon = static_cast<T>(1e-6))
{
  return Distance(inLHS, inRHS) < inEpsilon;
}

template <typename T>
constexpr bool IsNormalized(const T& inV)
{
  using ValueType = typename T::ValueType;
  return VeryEqual(SqLength(inV), static_cast<ValueType>(1));
}

template <typename T>
[[nodiscard]] constexpr T Normalized(const T& inV)
{
  const auto length = Length(inV);
  EXPECTS(length != 0);
  return inV / length;
}

template <typename T>
[[nodiscard]] constexpr T NormalizedSafe(const T& inV)
{
  const auto sq_length = SqLength(inV);
  if (sq_length == 0.0f)
    return inV;
  const auto length = std::sqrt(sq_length);
  return inV / length;
}

template <typename T>
constexpr Quat<T> ToQuaternion(const Mat4<T>& inRotationMat)
{
  const auto& m = inRotationMat;
  const auto fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
  const auto fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
  const auto fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
  const auto fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

  auto biggestIndex = 0;
  auto fourBiggestSquaredMinus1 = fourXSquaredMinus1;
  if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourYSquaredMinus1;
    biggestIndex = 1;
  }
  if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourZSquaredMinus1;
    biggestIndex = 2;
  }
  if (fourWSquaredMinus1 > fourBiggestSquaredMinus1)
  {
    fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    biggestIndex = 3;
  }

  const auto biggestVal = std::sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
  const auto mult = static_cast<T>(0.25) / biggestVal;
  switch (biggestIndex)
  {
  case 0:
    return Quat<T> { biggestVal, (m[1][0] + m[0][1]) * mult, (m[0][2] + m[2][0]) * mult, (m[2][1] - m[1][2]) * mult };

  case 1:
    return Quat<T> { (m[1][0] + m[0][1]) * mult, biggestVal, (m[2][1] + m[1][2]) * mult, (m[0][2] - m[2][0]) * mult };

  case 2:
    return Quat<T> { (m[0][2] + m[2][0]) * mult, (m[2][1] + m[1][2]) * mult, biggestVal, (m[1][0] - m[0][1]) * mult };

  default:
    return Quat<T> { (m[2][1] - m[1][2]) * mult, (m[0][2] - m[2][0]) * mult, (m[1][0] - m[0][1]) * mult, biggestVal };
  }
}

template <typename T>
constexpr Vec3<T> Direction(const Quat<T>& inQuat)
{
  EXPECTS(IsNormalized(inQuat));
  const auto direction = Normalized(inQuat * Forward<Vec3<T>>());
  ENSURES(IsNormalized(direction));
  return direction;
}

template <typename T, std::size_t N>
constexpr Vec3<T> Direction(const Segment<T, N>& inSegment)
{
  const auto direction = NormalizedSafe(inSegment.GetVector());
  return direction;
}

template <typename T>
constexpr T Pitch(const Quat<T>& inQuat)
{
  return (std::atan2((static_cast<T>(2) * (inQuat[1] * inQuat[2] + inQuat[3] * inQuat[0])),
      (inQuat[3] * inQuat[3] - inQuat[0] * inQuat[0] - inQuat[1] * inQuat[1] + inQuat[2] * inQuat[2])));
}

template <typename T>
constexpr T Yaw(const Quat<T>& inQuat)
{
  return std::asin(Clamp(static_cast<T>(-2) * (inQuat[0] * inQuat[2] - inQuat[3] * inQuat[1]),
      static_cast<T>(-1),
      static_cast<T>(1)));
}

template <typename T>
constexpr T Roll(const Quat<T>& inQuat)
{
  return static_cast<T>(std::atan2((static_cast<T>(2) * (inQuat[0] * inQuat[1] + inQuat[3] * inQuat[2])),
      (inQuat[3] * inQuat[3] + inQuat[0] * inQuat[0] - inQuat[1] * inQuat[1] - inQuat[2] * inQuat[2])));
}

template <typename T>
constexpr T DegreeToRad(const T& inV)
{
  constexpr T Factor = (Pi<T>() / 180.0);
  return inV * Factor;
}

template <typename T>
constexpr T RadToDegree(const T& inV)
{
  constexpr T Factor = (180.0 / Pi<T>());
  return inV * Factor;
}

template <typename T>
constexpr Vec3<T> AngleAxis(const Quat<T>& inQuat)
{
  EXPECTS(IsNormalized(inQuat));

  const auto angle = static_cast<T>(2) * std::acos(inQuat[3]);
  const auto wSquared = inQuat[3] * inQuat[3];
  const auto oneMinusWSquared = (static_cast<T>(1) - wSquared);
  if (VeryEqual(oneMinusWSquared, static_cast<T>(0)))
    return Zero<Vec3<T>>();

  const auto sqrt = std::sqrt(oneMinusWSquared);
  const auto axis = Vec3<T>(inQuat[0] / sqrt, inQuat[1] / sqrt, inQuat[2] / sqrt);
  return Normalized(axis) * angle;
}

template <typename T>
constexpr Quat<T> Conjugated(const Quat<T>& inQuat)
{
  return -inQuat;
}

template <typename T>
constexpr Vec3<T> Forward(const Quat<T>& inQuat)
{
  EXPECTS(IsNormalized(inQuat));

  return Direction(inQuat);
}

template <typename T>
constexpr std::tuple<Vec3<T>, Vec3<T>, Vec3<T>> Axes(const Vec3<T>& inForwardVectorNormalized,
    const Vec3<T>& inUpVectorNormalized = Up<Vec3<T>>())
{
  EXPECTS(IsNormalized(inForwardVectorNormalized));
  EXPECTS(IsNormalized(inUpVectorNormalized));

  const auto forward_vector = inForwardVectorNormalized;

  auto up_vector = inUpVectorNormalized;
  auto right_vector = Zero<Vec3f>();
  if (VeryParallel(forward_vector, up_vector))
  {
    right_vector = Right<Vec3f>();
    if (VeryParallel(right_vector, forward_vector))
      right_vector = Normalized(Vec3f(0.5f, 0.5f, 0.0f));

    up_vector = Normalized(Cross(right_vector, forward_vector));
    right_vector = Cross(forward_vector, up_vector);
  }
  else
  {
    right_vector = Normalized(Cross(forward_vector, up_vector));
    up_vector = Cross(right_vector, forward_vector);
  }

  ENSURES(IsNormalized(forward_vector));
  ENSURES(IsNormalized(up_vector));
  ENSURES(IsNormalized(right_vector));

  ENSURES(VeryPerpendicular(forward_vector, up_vector));
  ENSURES(VeryPerpendicular(forward_vector, right_vector));
  ENSURES(VeryPerpendicular(up_vector, right_vector));

  return { right_vector, up_vector, forward_vector };
}

template <typename T>
constexpr std::tuple<Vec3<T>, Vec3<T>, Vec3<T>> Axes(const Quat<T>& inOrientation)
{
  EXPECTS(IsNormalized(inOrientation));
  return { inOrientation * Right<Vec3<T>>(), inOrientation * Up<Vec3<T>>(), inOrientation * Forward<Vec3<T>>() };
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Transposed(const SquareMat<T, N>& inMat)
{
  SquareMat<T, N> transposed;
  for (std::size_t row = 0; row < N; ++row)
  {
    for (std::size_t col = 0; col < N; ++col) { transposed[row][col] = inMat[col][row]; }
  }
  return transposed;
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N - 1>
Cofactor(const SquareMat<T, N>& inMat, const std::size_t inRowToRemove, const std::size_t inColumnToRemove)
{
  std::size_t cofactor_matrix_row = 0;
  std::size_t cofactor_matrix_col = 0;
  SquareMat<T, N - 1> cofactor_matrix;
  for (std::size_t row = 0; row < N; row++)
  {
    if (row == inRowToRemove)
      continue;

    for (std::size_t col = 0; col < N; col++)
    {
      if (col == inColumnToRemove)
        continue;

      cofactor_matrix[cofactor_matrix_row][cofactor_matrix_col] = inMat[row][col];

      ++cofactor_matrix_col;
      if (cofactor_matrix_col == N - 1)
      {
        ++cofactor_matrix_row;
        cofactor_matrix_col = 0;
      }
    }
  }
  return cofactor_matrix;
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Adjoint(const SquareMat<T, N>& inMat)
{
  if (N == 1)
    return SquareMat<T, N>(static_cast<T>(1));

  SquareMat<T, N> adjoint;
  for (std::size_t row = 0; row < N; row++)
  {
    for (std::size_t col = 0; col < N; col++)
    {
      const auto cofactor = Cofactor(inMat, row, col);
      const auto sign = static_cast<T>(((row + col) % 2 == 0) ? 1 : -1);
      adjoint[col][row] = sign * Determinant(cofactor);
    }
  }
  return adjoint;
}

template <typename T, std::size_t N>
constexpr T Determinant(const SquareMat<T, N>& inMat)
{
  if constexpr (N == 1)
    return inMat[0][0];
  else
  {
    auto sign = static_cast<T>(1);
    auto determinant = static_cast<T>(0);
    for (std::size_t f = 0; f < N; f++)
    {
      const auto cofactor = Cofactor(inMat, 0, f);
      determinant += sign * inMat[0][f] * Determinant(cofactor);
      sign = -sign;
    }
    return determinant;
  }
}

template <typename T>
constexpr auto Inverted(const T& inValue)
{
  if constexpr (IsVec_v<T> || IsQuat_v<T>)
  {
    return -inValue;
  }
  else if constexpr (IsMat_v<T>)
  {
    const auto determinant = Determinant(inValue);
    if (determinant == 0)
      THROW_EXCEPTION("Singular matrix (determinant is 0), can not compute its inverse");

    const auto adjoint = Adjoint(inValue);

    const auto inverse = adjoint / determinant;
    return inverse;
  }
  else
  {
    static_assert("Inverted not implemented for this type.");
  }
}

template <typename T>
constexpr Quat<T> AngleAxis(const T& inAngleRads, const Vec3<T>& inAxisNormalized)
{
  EXPECTS(IsNormalized(inAxisNormalized));

  const auto half_angle = inAngleRads * static_cast<T>(0.5);
  const auto half_angle_sin = std::sin(half_angle);
  auto result_quat = Quat<T>(inAxisNormalized[0] * half_angle_sin,
      inAxisNormalized[1] * half_angle_sin,
      inAxisNormalized[2] * half_angle_sin,
      std::cos(half_angle));

  ENSURES(IsNormalized(result_quat));
  return result_quat;
}

template <typename T>
constexpr auto RandomUnit()
{
  if constexpr (IsNumber_v<T>)
  {
    return static_cast<T>(static_cast<double>(rand()) / RAND_MAX);
  }
  else if constexpr (IsQuat_v<T>)
  {
    using ValueType = typename T::ValueType;
    return AngleAxis(RandomUnit<ValueType>() * FullCircleRads<ValueType>(), RandomUnit<Vec3<ValueType>>());
  }
  else
  {
    using ValueType = typename T::ValueType;

    T result;
    for (auto& value : result) { value = RandomUnit<ValueType>(); }
    result = Normalized(result);
    return result;
  }
}

template <typename T>
// Min always included. Max excluded in integer types, included in real types.
constexpr auto Random(const T& inMin, const T& inMax)
{
  if constexpr (IsNumber_v<T>)
  {
    return static_cast<T>(RandomUnit<double>() * (inMax - inMin) + inMin);
  }
  else
  {
    using ValueType = typename T::ValueType;

    T result;
    auto min_it = inMin.begin();
    auto max_it = inMax.begin();
    auto result_it = result.begin();
    while (result_it != result.end())
    {
      *result_it = Random<ValueType>(*min_it, *max_it);
      ++min_it;
      ++max_it;
      ++result_it;
    }
    return result;
  }
}

template <typename T>
constexpr auto RandomSign()
{
  if constexpr (IsNumber_v<T>)
  {
    return (rand() % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
  }
  else
  {
    using ValueType = typename T::ValueType;
    T result;
    for (auto& value : result) value = RandomSign<ValueType>();
    return result;
  }
}

template <typename T>
constexpr T
Map(const T& inValue, const T& inSourceBegin, const T& inSourceEnd, const T& inTargetBegin, const T& inTargetEnd)
{
  const auto source_range = (inSourceEnd - inSourceBegin);
  const auto target_range = (inTargetEnd - inTargetBegin);
  const auto source_progress = ((inValue - inSourceBegin) / source_range);
  return (source_progress * target_range) + inTargetBegin;
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Diagonal(const T& inDiagonalValue)
{
  SquareMat<T, N> diagonal_matrix(static_cast<T>(0));
  for (std::size_t i = 0; i < N; ++i) { diagonal_matrix[i][i] = inDiagonalValue; }
  return diagonal_matrix;
}

template <typename T>
constexpr auto Identity()
{
  if constexpr (IsMat_v<T>)
  {
    using ValueType = typename T::ValueType;
    return Diagonal<ValueType, T::NumRows>(static_cast<ValueType>(1));
  }
  else
  {
    return T {};
  }
}

template <typename T>
constexpr Quat<T> FromEulerAngles(const Vec3<T>& inEulerAnglesRads)
{
  const auto qx = Quat<T>::AngleAxis(inEulerAnglesRads[0], Right<Vec3<T>>());
  const auto qy = Quat<T>::AngleAxis(inEulerAnglesRads[1], Up<Vec3<T>>());
  const auto qz = Quat<T>::AngleAxis(inEulerAnglesRads[2], Forward<Vec3<T>>());
  return Normalized(qz * qy * qx);
}

template <typename T, std::size_t N>
constexpr Vec<T, N> FromTo(const Vec<T, N>& inFrom, const Vec<T, N>& inTo)
{
  return (inTo - inFrom);
}

template <typename T>
constexpr Quat<T> FromTo(const Vec3<T>& inFromNormalized, const Vec3<T>& inToNormalized)
{
  EXPECTS(IsNormalized(inFromNormalized));
  EXPECTS(IsNormalized(inToNormalized));

  const auto from_to_dot = Dot(inFromNormalized, inToNormalized);
  if (from_to_dot >= 1.0)
    return Identity<Quat<T>>();

  if (from_to_dot <= -1.0)
  {
    auto axis = Cross(Right<Vec3<T>>(), inFromNormalized);
    if (axis.SqLength() == 0)
      axis = Cross(Up<Vec3<T>>(), inFromNormalized);
    axis = Normalized(axis);
    return Quat<T>(axis[0], axis[1], axis[2], 0.0);
  }

  const auto s = static_cast<T>(std::sqrt((1 + from_to_dot) * 2));
  const auto inverse_of_s = (static_cast<T>(1.0) / s);
  const auto c = Cross(inFromNormalized, inToNormalized) * inverse_of_s;
  return Normalized(Vec3<T> { c[0], c[1], c[2], s * 0.5 });
}

template <typename T>
constexpr bool VeryParallel(const Vec3<T>& inDirection0, const Vec3<T>& inDirection1)
{
  return VeryEqual(Abs(Dot(inDirection0, inDirection1)), static_cast<T>(1));
}

template <typename T>
constexpr bool VeryPerpendicular(const Vec3<T>& inDirection0, const Vec3<T>& inDirection1)
{
  return VeryEqual(Abs(Dot(inDirection0, inDirection1)), static_cast<T>(0));
}

template <typename T>
constexpr bool Between(const T& inValue, const T& inMin, const T& inMax)
{
  return (inValue >= inMin) && (inValue <= inMax);
}

template <typename T>
constexpr Quat<T> LookInDirection(const Vec3<T>& inForwardNormalized, const Vec3<T>& inUpNormalized = Up<Vec3<T>>())
{
  EXPECTS(IsNormalized(inForwardNormalized));
  EXPECTS(IsNormalized(inUpNormalized));

  const auto [x_axis, y_axis, inverted_z_axis] = Axes(inForwardNormalized, inUpNormalized);
  const auto z_axis = -inverted_z_axis;

  auto rotation_basis_matrix = Mat4<T> { Vec4<T> { x_axis[0], y_axis[0], z_axis[0], static_cast<T>(0) },
    Vec4<T> { x_axis[1], y_axis[1], z_axis[1], static_cast<T>(0) },
    Vec4<T> { x_axis[2], y_axis[2], z_axis[2], static_cast<T>(0) },
    Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) } };

  const auto quat_result = Normalized(ToQuaternion(rotation_basis_matrix));
  ENSURES(IsNormalized(quat_result));
  return quat_result;
}

template <std::size_t N, typename T>
constexpr SquareMat<T, N> NormalMat(const SquareMat<T, N>& inModelViewMatrix)
{
  return Transposed(Inverted(inModelViewMatrix));
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N + 1> TranslationMat(const Vec<T, N>& inTranslation)
{
  auto translation_matrix = Identity<SquareMat<T, N + 1>>();
  for (std::size_t i = 0; i < N; ++i) { translation_matrix[i][N] = inTranslation[i]; }
  return translation_matrix;
}

template <typename T>
constexpr std::enable_if_t<IsNumber_v<T>, SquareMat<T, 3>> RotationMat(const T inAngle)
{
  return SquareMat<T, 3> { Vec3<T> { std::cos(inAngle), -std::sin(inAngle), static_cast<T>(0) },
    Vec3<T> { std::sin(inAngle), std::cos(inAngle), static_cast<T>(0) },
    Vec3<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) } };
}

template <typename T>
constexpr SquareMat<T, 4> RotationMat(const Quat<T>& inRotation)
{
  const auto& q = inRotation;
  const auto qxx { q[0] * q[0] };
  const auto qyy { q[1] * q[1] };
  const auto qzz { q[2] * q[2] };
  const auto qxz { q[0] * q[2] };
  const auto qxy { q[0] * q[1] };
  const auto qyz { q[1] * q[2] };
  const auto qwx { q[3] * q[0] };
  const auto qwy { q[3] * q[1] };
  const auto qwz { q[3] * q[2] };

  return Mat4<T> { Vec4<T> { static_cast<T>(1 - 2 * (qyy + qzz)),
                       static_cast<T>(2 * (qxy - qwz)),
                       static_cast<T>(2 * (qxz + qwy)),
                       static_cast<T>(0) },
    Vec4<T> { static_cast<T>(2 * (qxy + qwz)),
        static_cast<T>(1 - 2 * (qxx + qzz)),
        static_cast<T>(2 * (qyz - qwx)),
        static_cast<T>(0) },
    Vec4<T> { static_cast<T>(2 * (qxz - qwy)),
        static_cast<T>(2 * (qyz + qwx)),
        static_cast<T>(1 - 2 * (qxx + qyy)),
        static_cast<T>(0) },
    Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) } };
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N + 1> ScaleMat(const Vec<T, N>& inScale)
{
  auto scale_matrix = Identity<SquareMat<T, N + 1>>();
  for (std::size_t i = 0; i < N; ++i) { scale_matrix[i][i] = inScale[i]; }
  scale_matrix[N][N] = static_cast<T>(1);
  return scale_matrix;
}

template <typename T>
constexpr Vec2<T> Rotated(const Vec2<T>& inVec, const AngleRads inAngle)
{
  return Vec2<T> { inVec[0] * std::cos(inAngle) - inVec[1] * std::sin(inAngle),
    inVec[0] * std::sin(inAngle) + inVec[1] * std::cos(inAngle) };
}

template <typename T>
constexpr Vec3<T> Rotated(const Vec3<T>& inVec, const Quat<T>& inRotation)
{
  return inRotation * inVec;
}

template <typename T>
constexpr Mat4<T> PerspectiveMat(const T inAngleOfViewRads, const T inAspectRatio, const T inZNear, const T inZFar)
{
  const T tanHalfFovy = std::tan(inAngleOfViewRads / static_cast<T>(2));

  Mat4<T> perspective_matrix = {};
  perspective_matrix[0][0] = static_cast<T>(1) / (inAspectRatio * tanHalfFovy);
  perspective_matrix[1][1] = static_cast<T>(1) / (tanHalfFovy);
  perspective_matrix[2][2] = -(inZFar + inZNear) / (inZFar - inZNear);
  perspective_matrix[3][2] = -static_cast<T>(1);
  perspective_matrix[2][3] = -(static_cast<T>(2) * inZFar * inZNear) / (inZFar - inZNear);
  return perspective_matrix;
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N + 1> OrthographicMat(const Vec<T, N>& inMin, const Vec<T, N>& inMax)
{
  SquareMat<T, N + 1> orthographic_matrix;
  for (std::size_t i = 0; i < N; ++i)
  {
    orthographic_matrix[i][i] = static_cast<T>(2) / (inMax[i] - inMin[i]);
    orthographic_matrix[i][N] = -static_cast<T>(inMax[i] + inMin[i]) / static_cast<T>(inMax[i] - inMin[i]);
  }
  orthographic_matrix[N][N] = static_cast<T>(1);
  return orthographic_matrix;
}

template <typename T, typename TInterpolationFactor>
constexpr T Lerp(const T& inFrom, const T& inTo, const TInterpolationFactor& t)
{
  return inFrom + (inTo - inFrom) * t;
}

template <typename T, typename TQuat>
constexpr TQuat SLerp(const TQuat& inFrom, const TQuat& inTo, const T& inT)
{
  auto to = inTo;
  auto cosTheta = Dot(inFrom, inTo);

  // If cosTheta < 0, the interpolation will take the long way around the
  // sphere. To fix this, one quat must be negated.
  if (cosTheta < static_cast<T>(0))
  {
    to = -inTo;
    cosTheta = -cosTheta;
  }

  if (cosTheta > static_cast<T>(1) - static_cast<T>(0.01))
  {
    return { Lerp(inFrom[0], to[0], inT),
      Lerp(inFrom[1], to[1], inT),
      Lerp(inFrom[2], to[2], inT),
      Lerp(inFrom[3], to[3], inT) };
  }
  else
  {
    const auto angle = std::acos(cosTheta);
    return (std::sin((static_cast<T>(1) - inT) * angle) * inFrom + std::sin(inT * angle) * to) / std::sin(angle);
  }
}

// Swizzling
// clang-format off
template <typename T, std::size_t N> constexpr Vec2<T> XX(const Vec<T, N> &inV) { return Vec2<T>{inV[0], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec2<T> XY(const Vec<T, N> &inV) { return Vec2<T>{inV[0], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec2<T> XZ(const Vec<T, N> &inV) { return Vec2<T>{inV[0], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec2<T> YX(const Vec<T, N> &inV) { return Vec2<T>{inV[1], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec2<T> YY(const Vec<T, N> &inV) { return Vec2<T>{inV[1], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec2<T> YZ(const Vec<T, N> &inV) { return Vec2<T>{inV[1], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec2<T> ZX(const Vec<T, N> &inV) { return Vec2<T>{inV[2], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec2<T> ZY(const Vec<T, N> &inV) { return Vec2<T>{inV[2], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec2<T> ZZ(const Vec<T, N> &inV) { return Vec2<T>{inV[2], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec2<T> X0(const Vec<T, N> &inV) { return Vec2<T>{inV[0], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec2<T> Y0(const Vec<T, N> &inV) { return Vec2<T>{inV[1], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec2<T> X1(const Vec<T, N> &inV) { return Vec2<T>{inV[0], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec2<T> Y1(const Vec<T, N> &inV) { return Vec2<T>{inV[1], static_cast<T>(1)}; }

template <typename T, std::size_t N> constexpr Vec3<T> XXX(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[0], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XXY(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[0], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XXZ(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[0], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XYX(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[1], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XYY(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[1], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XYZ(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[1], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XZX(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[2], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XZY(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[2], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XZZ(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[2], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YXX(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[0], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YXY(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[0], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YXZ(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[0], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YYX(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[1], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YYY(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[1], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YYZ(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[1], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YZX(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[2], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YZY(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[2], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> YZZ(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[2], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZXX(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[0], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZXY(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[0], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZXZ(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[0], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZYX(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[1], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZYY(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[1], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZYZ(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[1], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZZX(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[2], inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZZY(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[2], inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZZZ(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[2], inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> XX0(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[0], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> XY0(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[1], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> XZ0(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[2], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YX0(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[0], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YY0(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[1], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YZ0(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[2], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZX0(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[0], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZY0(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[1], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZZ0(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[2], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec3<T> XX1(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[0], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> XY1(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[1], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> XZ1(const Vec<T, N> &inV) { return Vec3<T>{inV[0], inV[2], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YX1(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[0], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YY1(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[1], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> YZ1(const Vec<T, N> &inV) { return Vec3<T>{inV[1], inV[2], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZX1(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[0], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZY1(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[1], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> ZZ1(const Vec<T, N> &inV) { return Vec3<T>{inV[2], inV[2], static_cast<T>(1)}; }
template <typename T, std::size_t N> constexpr Vec3<T> X0X(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(0), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> X0Y(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(0), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> X0Z(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(0), inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y0X(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(0), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y0Y(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(0), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y0Z(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(0), inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z0X(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(0), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z0Y(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(0), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z0Z(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(0), inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> X1X(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(1), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> X1Y(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(1), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> X1Z(const Vec<T, N> &inV) { return Vec3<T>{inV[0], static_cast<T>(1), inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y1X(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(1), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y1Y(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(1), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Y1Z(const Vec<T, N> &inV) { return Vec3<T>{inV[1], static_cast<T>(1), inV[2]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z1X(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(1), inV[0]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z1Y(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(1), inV[1]}; }
template <typename T, std::size_t N> constexpr Vec3<T> Z1Z(const Vec<T, N> &inV) { return Vec3<T>{inV[2], static_cast<T>(1), inV[2]}; }

template <typename T, std::size_t N> constexpr Vec4<T> XYZ0(const Vec<T, N> &inV) { return Vec4<T>{inV[0], inV[1], inV[2], static_cast<T>(0)}; }
template <typename T, std::size_t N> constexpr Vec4<T> XYZ1(const Vec<T, N> &inV) { return Vec4<T>{inV[0], inV[1], inV[2], static_cast<T>(1)}; }
// clang-format on
}