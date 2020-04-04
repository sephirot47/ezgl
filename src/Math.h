#pragma once

#include <cmath>
#include <cstdint>

#include "Quat.h"
#include "Mat.h"
#include "Vec.h"

namespace egl
{

template <typename T = float>
constexpr Vec3<T> Right() { return { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) }; }
template <typename T = float>
constexpr Vec3<T> Left() { return { static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0) }; }
template <typename T = float>
constexpr Vec3<T> Up() { return { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) }; }
template <typename T = float>
constexpr Vec3<T> Down() { return { static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0) }; }
template <typename T = float>
constexpr Vec3<T> Forward() { return { static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1) }; }
template <typename T = float>
constexpr Vec3<T> Back() { return { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }; }

template <typename T>
constexpr T Abs(const T& inValue)
{
    return std::abs(inValue);
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
    return (inValue < 0 ? static_cast<T>(-1) : 1);
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
    auto fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    const auto biggestVal = std::sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
    const auto mult = static_cast<T>(0.25) / biggestVal;

    Quat<T> res;
    switch (biggestIndex)
    {
    case 0:
        return { (m.c1[2] - m.c2[1]) * mult,
            (m.c2[0] - m.c0[2]) * mult,
            (m.c0[1] - m.c1[0]) * mult,
            biggestVal };
        break;
    case 1:
        return { biggestVal,
            (m.c0[1] + m.c1[0]) * mult,
            (m.c2[0] + m.c0[2]) * mult,
            (m.c1[2] - m.c2[1]) * mult };
        break;
    case 2:
        return { (m.c0[1] + m.c1[0]) * mult,
            biggestVal,
            (m.c1[2] + m.c2[1]) * mult,
            (m.c2[0] - m.c0[2]) * mult };
        break;
    default:
        return { (m.c2[0] + m.c0[2]) * mult,
            (m.c1[2] + m.c2[1]) * mult,
            biggestVal,
            (m.c0[1] - m.c1[0]) * mult };
        break;
    }
}

template <typename TMat>
constexpr TMat Diagonal(const typename TMat::ValueType inDiagonalValue)
{
    static_assert(TMat::NumRows == TMat::NumCols);

    constexpr auto N = TMat::NumRows;
    using ValueType = typename TMat::ValueType;

    TMat diagonal_matrix(static_cast<ValueType>(0));
    for (std::size_t i = 0; i < N; ++i)
    {
        diagonal_matrix[i][i] = inDiagonalValue;
    }
    return diagonal_matrix;
}

template <typename TMat>
constexpr TMat Identity()
{
    return Diagonal<TMat>(static_cast<typename TMat::ValueType>(1));
}

template <typename T>
constexpr Mat4<T> TranslationMat4(const Vec3<T>& inTranslation)
{
    return {
        { 1, 0, 0, inTranslation[0] },
        { 0, 1, 0, inTranslation[1] },
        { 0, 0, 1, inTranslation[2] },
        { 0, 0, 0, 1 }
    };
}

template <typename T>
constexpr Mat4<T> RotationMat4(const Quat<T>& inRotation)
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

    return Mat4<T> {
        Vec4<T> { static_cast<T>(1 - 2 * (qyy + qzz)), static_cast<T>(2 * (qxy - qwz)), static_cast<T>(2 * (qxz + qwy)), static_cast<T>(0) },
        Vec4<T> { static_cast<T>(2 * (qxy + qwz)), static_cast<T>(1 - 2 * (qxx + qzz)), static_cast<T>(2 * (qyz - qwx)), static_cast<T>(0) },
        Vec4<T> { static_cast<T>(2 * (qxz - qwy)), static_cast<T>(2 * (qyz + qwx)), static_cast<T>(1 - 2 * (qxx + qyy)), static_cast<T>(0) },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }
    };
}

template <typename T>
constexpr Mat4<T> ScaleMat4(const Vec3<T>& inScale)
{
    return {
        { inScale[0], 0, 0, 0 },
        { 0, inScale[1], 0, 0 },
        { 0, 0, inScale[2], 0 },
        { 0, 0, 0, 1 }
    };
}

template <typename T, typename TT>
constexpr T Lerp(const T& inFrom, const T& inTo, const TT& t)
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

template <typename T, std::size_t N>
constexpr T SqLength(const Vec<T, N>& inV)
{
    return Dot(inV, inV);
}

template <typename T, std::size_t N>
constexpr T Length(const Vec<T, N>& inV)
{
    return std::sqrt(SqLength(inV));
}

template <typename T, std::size_t N>
[[nodiscard]] constexpr Vec<T, N> Normalized(const Vec<T, N>& inV)
{
    return inV / Length(inV);
}

template <typename T, std::size_t N>
void Normalize(Vec<T, N>& inV)
{
    inV = Normalized(inV);
}

template <typename T, std::size_t N>
constexpr T SqDistance(const Vec<T, N>& inLHS, const Vec<T, N>& inRHS)
{
    const auto diff = (inRHS - inLHS);
    return SqLength(diff, diff);
}

template <typename T, std::size_t N>
constexpr T Distance(const Vec<T, N>& inLHS, const Vec<T, N>& inRHS)
{
    return std::sqrt(SqDistance(inLHS, inRHS));
}

template <typename T, std::size_t N>
constexpr T Dot(const Vec<T, N>& inLHS, const Vec<T, N>& inRHS)
{
    T dot { 0 };
    for (std::size_t i = 0; i < N; ++i)
    {
        dot += inLHS[i] * inRHS[i];
    }
    return dot;
}

template <typename T>
constexpr Vec3<T> Cross(const Vec3<T>& inLHS, const Vec3<T>& inRHS)
{
    return Vec3<T> { inLHS[1] * inRHS[2] - inLHS[2] * inRHS[1],
        inLHS[2] * inRHS[0] - inLHS[0] * inRHS[2],
        inLHS[0] * inRHS[1] - inLHS[1] * inRHS[0] };
}
}