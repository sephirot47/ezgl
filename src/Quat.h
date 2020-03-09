#pragma once

#include <array>
#include <ostream>

namespace egl
{

template <typename T>
class Quat
{
public:
    constexpr Quat() noexcept;
    constexpr Quat(const T& inX, const T& inY, const T& inZ, const T& inW) noexcept;

    constexpr T Length() const;
    constexpr T SqLength() const;
    constexpr Quat<T> Conjugated() const;
    constexpr Quat<T> Normalized() const;
    constexpr Quat<T> Inversed() const;
    constexpr Vec3<T> GetEulerAngles() const;
    constexpr T GetPitch() const;
    constexpr T GetYaw() const;
    constexpr T GetRoll() const;
    constexpr Vec3<T> GetAngleAxis() const;

    static constexpr Quat<T> FromEulerAngles(const Vec3<T>& inEulerAnglesRads);
    static constexpr Vec3<T> GetEulerAngles(const Quat<T>& inQuat);
    static constexpr Quat<T> FromTo(const Vec3<T>& inFrom, const Vec3<T>& inTo);
    static constexpr Quat<T> LookDirection(const Vec3<T>& inForward, const Vec3<T>& inUp = Vec3<T>::Up());
    static constexpr Quat<T> AngleAxis(const T& inAngleRads, const Vec3<T>& inAxis);

    // Operators
    constexpr T& operator[](std::size_t i);
    constexpr const T& operator[](std::size_t i) const;
    constexpr bool operator==(const Quat<T>& inRHS) const;
    constexpr bool operator!=(const Quat<T>& inRHS) const;
    constexpr Quat<T> operator+(const Quat<T>& inRHS) const;
    constexpr Quat<T> operator-(const Quat<T>& inRHS) const;
    constexpr Quat<T> operator*(const T& inRHS) const;
    constexpr Quat<T> operator*(const Quat<T>& inRHS) const;
    constexpr Vec3<T> operator*(const Vec3<T>& inRHS) const;
    constexpr Vec4<T> operator*(const Vec4<T>& inRHS) const;
    constexpr Quat<T> operator/(const T& inRHS) const;
    void operator+=(const Quat<T>& inRHS);
    void operator-=(const Quat<T>& inRHS);
    void operator*=(const T& inRHS);
    void operator*=(const Quat<T>& inRHS);
    void operator/=(const T& inRHS);
    constexpr Quat<T> operator-() const;

private:
    std::array<T, 4> mComponents;
};

template <typename T>
Quat<T> operator*(const T& a, const Quat<T>& inRHS);

template <typename T>
Quat<T> operator/(const T& a, const Quat<T>& inRHS);

template <typename T>
std::ostream& operator<<(std::ostream& log, const Quat<T>& q);

using Quati = Quat<int>;
using Quatf = Quat<float>;
using Quatd = Quat<double>;

}

#include "Quat.tcc"
