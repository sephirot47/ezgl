#include "Quat.h"

#include <cmath>

#include "Macros.h"

namespace egl
{
template <typename T>
constexpr Quat<T>::Quat() noexcept
    : mComponents { 0, 0, 0, 0 }
{
}

template <typename T>
constexpr Quat<T>::Quat(const T& inX, const T& inY, const T& inZ, const T& inW) noexcept
    : mComponents { inX, inY, inZ, inW }
{
}

template <typename T>
constexpr T Quat<T>::Length() const
{
    return std::sqrt(SqLength());
}

template <typename T>
constexpr T Quat<T>::SqLength() const
{
    return ((*this)[0] * (*this)[0] + (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2] + (*this)[3] * (*this)[3]);
}

template <typename T>
constexpr Quat<T> Quat<T>::Conjugated() const
{
    return -(*this);
}

template <typename T>
constexpr Quat<T> Quat<T>::Normalized() const
{
    const auto length = Length();
    EXPECTS(length != 0);
    return (*this) / length;
}

template <typename T>
constexpr Quat<T> Quat<T>::Inversed() const
{
    return Conjugated() / SqLength();
}

template <typename T>
constexpr Vec3<T> Quat<T>::GetEulerAngles() const
{
    return Quat<T>::GetEulerAngles(*this);
}

template <typename T>
constexpr T Quat<T>::GetPitch() const
{
    return static_cast<T>(std::atan2((static_cast<T>(2) * ((*this)[1] * (*this)[2] + (*this)[3] * (*this)[0])),
        ((*this)[3] * (*this)[3] - (*this)[0] * (*this)[0] - (*this)[1] * (*this)[1] + (*this)[2] * (*this)[2])));
}

template <typename T>
constexpr T Quat<T>::GetYaw() const
{
    return std::asin(Clamp(static_cast<T>(-2) * ((*this)[0] * (*this)[2] - (*this)[3] * (*this)[1]),
        static_cast<T>(-1),
        static_cast<T>(1)));
}

template <typename T>
constexpr T Quat<T>::GetRoll() const
{
    return static_cast<T>(std::atan2((static_cast<T>(2) * ((*this)[0] * (*this)[1] + (*this)[3] * (*this)[2])),
        ((*this)[3] * (*this)[3] + (*this)[0] * (*this)[0] - (*this)[1] * (*this)[1] - (*this)[2] * (*this)[2])));
}

template <typename T>
constexpr Vec3<T> Quat<T>::GetAngleAxis() const
{
    const auto angle = 2 * std::acos((*this)[3]);
    const auto w2 = (*this)[3] * (*this)[3];
    const auto sqrt = std::sqrt(1.0f - w2);

    Vec3<T> axis;
    if (sqrt != 0)
    {
        axis = Vec3<T>((*this)[0] / sqrt, (*this)[1] / sqrt, (*this)[2] / sqrt);
    }
    else
    {
        axis = Vec3<T>::Zero();
    }

    return axis.NormalizedSafe() * angle;
}

template <typename T>
constexpr Quat<T> Quat<T>::FromEulerAngles(const Vec3<T>& inEulerAnglesRads)
{
    const auto qx = Quat<T>::AngleAxis(inEulerAnglesRads[0], Vec3<T>::Right());
    const auto qy = Quat<T>::AngleAxis(inEulerAnglesRads[1], Vec3<T>::Up());
    const auto qz = Quat<T>::AngleAxis(inEulerAnglesRads[2], Vec3<T>::Forward());
    return (qz * qy * qx).Normalized();
}

template <typename T>
constexpr Vec3<T> Quat<T>::GetEulerAngles(const Quat<T>& inQuat)
{
    return Vec3<T>(inQuat.GetPitch(), inQuat.GetYaw(), inQuat.GetRoll()).ToDegrees();
}

template <typename T>
constexpr Quat<T> Quat<T>::FromTo(const Vec3<T>& inFrom, const Vec3<T>& inTo)
{
    const auto v0 = inFrom.Normalized();
    const auto v1 = inTo.Normalized();

    const auto d = Dot(v0, v1);
    if (d >= 1.0)
    {
        return Quat<T>::Identity();
    }
    else if (d <= -1.0)
    {
        auto axis = Cross(Vec3<T>::Right(), v0);
        if (axis.Length() == 0)
        {
            axis = Vec3<T>::Up();
            axis = Cross(axis, v0);
        }

        return Quat<T>(axis[0], axis[1], axis[2], 0.0).Normalized();
    }

    const auto s = static_cast<T>(std::sqrt((1 + d) * 2));
    const auto invs = (static_cast<T>(1.0) / s);
    const auto c = Cross(v0, v1) * invs;
    return Vec3<T> { c[0], c[1], c[2], s * 0.5 }.Normalized();
}

template <typename T>
constexpr Quat<T> Quat<T>::LookDirection(const Vec3<T>& inForward, const Vec3<T>& inUp)
{
    const auto forward = inForward.Normalized();
    const auto up = inUp.Normalized();
    const auto forward_up_dot = Dot(forward, up);
    return ToQuaternion(Mat4<T>::LookAt(Vec3<T>::Zero(), forward, up).Inversed());
}

template <typename T>
constexpr Quat<T> Quat<T>::AngleAxis(const T& inAngleRads, const Vec3<T>& inAxis)
{
    const auto s = std::sin(inAngleRads * static_cast<T>(0.5));
    return Quat<T>(static_cast<T>(inAxis[0]) * s,
        static_cast<T>(inAxis[1]) * s,
        static_cast<T>(inAxis[2]) * s,
        std::cos(inAngleRads * static_cast<T>(0.5)))
        .Normalized();
}

template <typename T>
constexpr T& Quat<T>::operator[](std::size_t i)
{
    return mComponents[i];
}

template <typename T>
constexpr const T& Quat<T>::operator[](std::size_t i) const
{
    return mComponents[i];
}

template <typename T>
constexpr bool Quat<T>::operator==(const Quat<T>& inRHS) const
{
    return mComponents == inRHS.mComponents;
}

template <typename T>
constexpr bool Quat<T>::operator!=(const Quat<T>& inRHS) const
{
    return mComponents != inRHS.mComponents;
}

template <typename T>
constexpr Quat<T> Quat<T>::operator+(const Quat<T>& inRHS) const
{
    return { (*this)[0] + inRHS[0], (*this)[1] + inRHS[1], (*this)[2] + inRHS[2], (*this)[3] + inRHS[3] };
}

template <typename T>
constexpr Quat<T> Quat<T>::operator-(const Quat<T>& inRHS) const
{
    return { (*this)[0] - inRHS[0], (*this)[1] - inRHS[1], (*this)[2] - inRHS[2], (*this)[3] - inRHS[3] };
}

template <typename T>
constexpr Quat<T> Quat<T>::operator*(const T& inRHS) const
{
    return { (*this)[0] * inRHS[0], (*this)[1] * inRHS[1], (*this)[2] * inRHS[2], (*this)[3] * inRHS[3] };
}

template <typename T>
constexpr Quat<T> Quat<T>::operator*(const Quat<T>& inRHS) const
{
    return { (*this)[3] * inRHS[0] + (*this)[0] * inRHS[3] + (*this)[1] * inRHS[2] - (*this)[2] * inRHS[1],
        (*this)[3] * inRHS[1] + (*this)[1] * inRHS[3] + (*this)[2] * inRHS[0] - (*this)[0] * inRHS[2],
        (*this)[3] * inRHS[2] + (*this)[2] * inRHS[3] + (*this)[0] * inRHS[1] - (*this)[1] * inRHS[0],
        (*this)[3] * inRHS[3] - (*this)[0] * inRHS[0] - (*this)[1] * inRHS[1] - (*this)[2] * inRHS[2] };
}

template <typename T>
constexpr Vec3<T> Quat<T>::operator*(const Vec3<T>& inRHS) const
{
    const auto q_vector = Vec3<T> { (*this)[0], (*this)[1], (*this)[2] };
    const auto uv(Cross(q_vector, inRHS));
    const auto uuv(Cross(q_vector, uv));
    return inRHS + ((uv * (*this)[3]) + uuv) * static_cast<T>(2);
}

template <typename T>
constexpr Vec4<T> Quat<T>::operator*(const Vec4<T>& inRHS) const
{
    const auto v3 = (*this) * Vec3<T> { inRHS[0], inRHS[1], inRHS[2] };
    return { v3[0], v3[1], v3[2], inRHS[3] };
}

template <typename T>
constexpr Quat<T> Quat<T>::operator/(const T& inRHS) const
{
    return { (*this)[0] / inRHS, (*this)[1] / inRHS, (*this)[2] / inRHS, (*this)[3] / inRHS };
}

template <typename T>
void Quat<T>::operator+=(const Quat<T>& inRHS)
{
    *this = *this + inRHS;
}

template <typename T>
void Quat<T>::operator-=(const Quat<T>& inRHS)
{
    *this = *this - inRHS;
}

template <typename T>
void Quat<T>::operator*=(const T& inRHS)
{
    *this = *this * inRHS;
}

template <typename T>
void Quat<T>::operator*=(const Quat<T>& inRHS)
{
    *this = *this * inRHS;
}

template <typename T>
void Quat<T>::operator/=(const T& inRHS)
{
    *this = *this / inRHS;
}

template <typename T>
constexpr Quat<T> Quat<T>::operator-() const
{
    return { -(*this)[0], -(*this)[1], -(*this)[2], -(*this)[3] };
}

template <typename T>
std::ostream& operator<<(std::ostream& log, const Quat<T>& q)
{
    log << "(" << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << ")";
    return log;
}
}
