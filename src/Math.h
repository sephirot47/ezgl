#pragma once

#include <cmath>
#include <cstdint>

#include "Quat.h"
#include "Mat.h"
#include "TypeTraits.h"
#include "Vec.h"

namespace egl
{

template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Right()
{
    static_assert(TVec::NumComponents >= 1);
    TVec result { static_cast<typename TVec::ValueType>(0) };
    result[0] = static_cast<typename TVec::ValueType>(1);
    return result;
}
template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Left() { return -Right<TVec>(); }

template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Up()
{
    static_assert(TVec::NumComponents >= 2);
    TVec result { static_cast<typename TVec::ValueType>(0) };
    result[1] = static_cast<typename TVec::ValueType>(1);
    return result;
}
template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Down() { return -Up<TVec>(); }

template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Forward()
{
    static_assert(TVec::NumComponents >= 3);
    TVec result { static_cast<typename TVec::ValueType>(0) };
    result[2] = static_cast<typename TVec::ValueType>(-1);
    return result;
}
template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Back() { return -Forward<TVec>(); }

template <typename TVecOrMat>
constexpr TVecOrMat All(const typename TVecOrMat::ValueType& inAllValue) { return TVecOrMat(inAllValue); }
template <typename TVecOrMat>
constexpr TVecOrMat Zero() { return All<TVecOrMat>(0); }
template <typename TVecOrMat>
constexpr TVecOrMat One() { return All<TVecOrMat>(1); }

template <typename T>
constexpr T Pi() { return static_cast<T>(3.14159265358979323846); }
template <typename T>
constexpr T QuarterPi() { return Pi<T>() / 4; }
template <typename T>
constexpr T HalfPi() { return Pi<T>() / 2; }
template <typename T>
constexpr T DoublePi() { return Pi<T>() * 2; }

template <typename T>
constexpr std::enable_if_t<IsNumber_v<T>, T> Abs(const T& inValue)
{
    return std::abs(inValue);
}

template <typename TVecOrMat>
constexpr std::enable_if_t<IsVecOrMat_v<TVecOrMat>, TVecOrMat> Abs(const TVecOrMat& inVecOrMat)
{
    auto result = inVecOrMat;
    for (auto& v : result)
    {
        v = Abs(v);
    }
    return result;
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
constexpr bool IsAlmostEqual(const T& inLHS, const T& inRHS, const T& inEpsilon = static_cast<T>(1e-6))
{
    return Abs(inLHS - inRHS) < inEpsilon;
}

template <typename T>
constexpr auto Dot(const T& inLHS, const T& inRHS)
{
    using ValueType = typename T::ValueType;

    auto dot = static_cast<ValueType>(0);
    for (std::size_t i = 0; i < T::NumComponents; ++i)
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
constexpr bool IsNormalized(const T& inV)
{
    using ValueType = typename T::ValueType;
    return IsAlmostEqual(SqLength(inV), static_cast<ValueType>(1));
}

template <typename T>
[[nodiscard]] constexpr T Normalized(const T& inV)
{
    const auto length = Length(inV);
    EXPECTS(length != 0);
    return inV / length;
}

template <typename T>
constexpr auto SqDistance(const T& inLHS, const T& inRHS)
{
    const auto diff = (inRHS - inLHS);
    return SqLength(diff, diff);
}

template <typename T, std::size_t N>
constexpr auto Distance(const T& inLHS, const T& inRHS)
{
    return std::sqrt(SqDistance(inLHS, inRHS));
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
        return Quat<T> { biggestVal,
            (m[1][0] + m[0][1]) * mult,
            (m[0][2] + m[2][0]) * mult,
            (m[2][1] - m[1][2]) * mult };

    case 1:
        return Quat<T> { (m[1][0] + m[0][1]) * mult,
            biggestVal,
            (m[2][1] + m[1][2]) * mult,
            (m[0][2] - m[2][0]) * mult };

    case 2:
        return Quat<T> { (m[0][2] + m[2][0]) * mult,
            (m[2][1] + m[1][2]) * mult,
            biggestVal,
            (m[1][0] - m[0][1]) * mult };

    default:
        return Quat<T> { (m[2][1] - m[1][2]) * mult,
            (m[0][2] - m[2][0]) * mult,
            (m[1][0] - m[0][1]) * mult,
            biggestVal };
    }
}

template <typename T>
constexpr Vec3<T> Direction(const Quat<T>& inQuat)
{
    return inQuat * Forward<Vec3<T>>();
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
    constexpr T Factor = (Pi<T>() / 180.0f);
    return inV * Factor;
}

template <typename T>
constexpr T RadToDegree(const T& inV)
{
    constexpr T Factor = (180.0f / Pi<T>());
    return inV * Factor;
}

template <typename T>
constexpr Vec3<T> AngleAxis(const Quat<T>& inQuat)
{
    const auto angle = 2 * std::acos(inQuat[3]);
    const auto wSquared = inQuat[3] * inQuat[3];
    const auto oneMinusWSquared = (1.0f - wSquared);
    if (oneMinusWSquared)
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

template <typename TVec>
constexpr std::enable_if_t<IsVec_v<TVec>, TVec> Inverted(const TVec& inVector)
{
    return static_cast<typename TVec::ValueType>(1.0) / inVector;
}

template <typename TQuat>
constexpr std::enable_if_t<IsQuat_v<TQuat>, TQuat> Inverted(const TQuat& inQuat)
{
    return Conjugated(inQuat) / SqLength(inQuat);
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Transposed(const SquareMat<T, N>& inMat)
{
    SquareMat<T, N> transposed;
    for (std::size_t row = 0; row < N; ++row)
    {
        for (std::size_t col = 0; col < N; ++col)
        {
            transposed[row][col] = inMat[col][row];
        }
    }
    return transposed;
}

template <typename T, std::size_t N>
constexpr SquareMat<T, N - 1> Cofactor(const SquareMat<T, N>& inMat, const std::size_t inRowToRemove, const std::size_t inColumnToRemove)
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

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Inverted(const SquareMat<T, N>& inMat)
{
    const auto determinant = Determinant(inMat);
    if (determinant == 0)
        THROW_EXCEPTION("Singular matrix (determinant is 0), can not compute its inverse");

    const auto adjoint = Adjoint(inMat);

    const auto inverse = adjoint / determinant;
    return inverse;
}

template <typename T>
const std::enable_if_t<std::is_arithmetic_v<T>, T> RandomUnit()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

template <typename T>
const std::enable_if_t<!std::is_arithmetic_v<T>, T> RandomUnit()
{
    using ValueType = typename T::ValueType;

    T result;
    for (ValueType& value : result)
    {
        value = RandomUnit<ValueType>();
    }

    result = Normalized(result);

    return result;
}

template <typename T>
// Min always included. Max excluded in integer types, included in real types.
const std::enable_if_t<std::is_arithmetic_v<T>, T> Random(const T& inMin, const T& inMax)
{
    return static_cast<T>(RandomUnit<double>() * (inMax - inMin) + inMin);
}

template <typename T>
const std::enable_if_t<!std::is_arithmetic_v<T>, T> Random(const T& inMin, const T& inMax)
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

template <typename T, std::size_t N>
constexpr SquareMat<T, N> Diagonal(const T& inDiagonalValue)
{
    SquareMat<T, N> diagonal_matrix(static_cast<T>(0));
    for (std::size_t i = 0; i < N; ++i)
    {
        diagonal_matrix[i][i] = inDiagonalValue;
    }
    return diagonal_matrix;
}

template <typename TMat>
constexpr std::enable_if_t<IsMat_v<TMat>, TMat> Identity()
{
    return Diagonal<TMat>(static_cast<typename TMat::ValueType>(1));
}

template <typename TQuat>
constexpr std::enable_if_t<IsQuat_v<TQuat>, TQuat> Identity()
{
    return TQuat { 0, 0, 0, 1 };
}

template <typename T>
constexpr Quat<T> FromEulerAngles(const Vec3<T>& inEulerAnglesRads)
{
    const auto qx = Quat<T>::AngleAxis(inEulerAnglesRads[0], Right<Vec3<T>>());
    const auto qy = Quat<T>::AngleAxis(inEulerAnglesRads[1], Up<Vec3<T>>());
    const auto qz = Quat<T>::AngleAxis(inEulerAnglesRads[2], Forward<Vec3<T>>());
    return Normalized(qz * qy * qx);
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
constexpr Quat<T> LookInDirection(const Vec3<T>& inForwardNormalized, const Vec3<T>& inUpNormalized = Up<Vec3<T>>())
{
    EXPECTS(IsNormalized(inForwardNormalized));
    EXPECTS(IsNormalized(inUpNormalized));

    const auto z_axis = -inForwardNormalized;
    const auto x_axis = Normalized(Cross(inUpNormalized, z_axis));
    const auto y_axis = Cross(z_axis, x_axis); // No need to normalize because both operands are already normalized and orthogonal
    assert(IsNormalized(y_axis));

    auto rotation_basis_matrix = Mat4<T> {
        Vec4<T> { x_axis[0], y_axis[0], z_axis[0], static_cast<T>(0) },
        Vec4<T> { x_axis[1], y_axis[1], z_axis[1], static_cast<T>(0) },
        Vec4<T> { x_axis[2], y_axis[2], z_axis[2], static_cast<T>(0) },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }
    };

    const auto quat_result = Normalized(ToQuaternion(rotation_basis_matrix));
    ENSURES(IsNormalized(quat_result));
    return quat_result;
}

template <typename T>
constexpr Mat4<T> TranslationMat4(const Vec3<T>& inTranslation)
{
    return Mat4<T> {
        Vec4<T> { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), inTranslation[0] },
        Vec4<T> { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), inTranslation[1] },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), inTranslation[2] },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }
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
    return Mat4<T> {
        Vec4<T> { inScale[0], static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) },
        Vec4<T> { static_cast<T>(0), inScale[1], static_cast<T>(0), static_cast<T>(0) },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), inScale[2], static_cast<T>(0) },
        Vec4<T> { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) }
    };
}

template <typename T>
constexpr Mat4<T> PerspectiveMat4(const T inAngleOfViewRads, const T inAspectRatio, const T inZNear, const T inZFar)
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

template <typename T>
constexpr Mat4<T> OrthographicMat4(const Vec3<T>& inMin, const Vec3<T>& inMax)
{
    Mat4<T> orthographic_matrix(1);
    orthographic_matrix[0][0] = static_cast<T>(2) / (inMax[0] - inMin[0]);
    orthographic_matrix[1][1] = static_cast<T>(2) / (inMax[1] - inMin[1]);
    orthographic_matrix[2][2] = -static_cast<T>(2) / (inMax[2] - inMin[2]);
    orthographic_matrix[0][3] = -static_cast<T>(inMax[0] + inMin[0]) / static_cast<T>(inMax[0] - inMin[0]);
    orthographic_matrix[1][3] = -static_cast<T>(inMax[1] + inMin[1]) / static_cast<T>(inMax[1] - inMin[1]);
    orthographic_matrix[2][3] = -static_cast<T>(inMax[2] + inMin[2]) / static_cast<T>(inMax[2] - inMin[2]);
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
}