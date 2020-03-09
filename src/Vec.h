#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <iostream>
#include <type_traits>
#include <initializer_list>

#include "VariadicRepeat.h"

namespace egl
{
template <typename T, std::size_t N>
class Vec
{
public:
    static_assert(N >= 2);

    using ValueType = T;
    static constexpr auto NumComponents = N;

    Vec() noexcept = default;

    constexpr explicit Vec(const T& inAllValue) noexcept;

    template <typename... TArgs, typename = std::enable_if_t<sizeof...(TArgs) == N>>
    constexpr explicit Vec(TArgs&&... inArgs) noexcept;

    constexpr Vec(const Vec&) noexcept = default;
    constexpr Vec& operator=(const Vec&) noexcept = default;

    constexpr Vec(Vec&&) noexcept = default;
    constexpr Vec& operator=(Vec&&) noexcept = default;

    ~Vec() = default;

    constexpr T Length() const;
    constexpr T SqLength() const;
    [[nodiscard]] constexpr Vec Normalized() const;
    void Normalize();

    static constexpr T Normalized(const Vec& inRHS);
    static constexpr T SqDistance(const Vec& inLHS, const Vec& inRHS);
    static constexpr T Distance(const Vec& inLHS, const Vec& inRHS);
    static constexpr T Dot(const Vec& inLHS, const Vec& inRHS);
    static constexpr Vec Cross(const Vec& inLHS, const Vec& inRHS); // Only for Vec3

    static constexpr Vec Zero();
    static constexpr Vec One();
    static constexpr Vec All(const T& inAllValue);

    T* Data();
    const T* Data() const;

    // Iterators
    constexpr std::array<T, N>::iterator begin();
    constexpr std::array<T, N>::iterator end();
    constexpr std::array<T, N>::const_iterator begin() const;
    constexpr std::array<T, N>::const_iterator end() const;
    constexpr std::array<T, N>::const_iterator cbegin() const;
    constexpr std::array<T, N>::const_iterator cend() const;

    // Operators
    constexpr T& operator[](std::size_t i);
    constexpr const T& operator[](std::size_t i) const;
    constexpr Vec<T, N> operator+(const Vec& inRHS) const;
    constexpr Vec<T, N> operator-(const Vec& inRHS) const;
    constexpr Vec<T, N> operator*(const Vec& inRHS) const;
    constexpr Vec<T, N> operator/(const Vec& inRHS) const;
    constexpr Vec<T, N> operator+(const T& inRHS) const;
    constexpr Vec<T, N> operator-(const T& inRHS) const;
    constexpr Vec<T, N> operator*(const T& inRHS) const;
    constexpr Vec<T, N> operator/(const T& inRHS) const;
    constexpr void operator+=(const Vec& inRHS);
    constexpr void operator-=(const Vec& inRHS);
    constexpr void operator*=(const Vec& inRHS);
    constexpr void operator/=(const Vec& inRHS);
    constexpr void operator+=(const T& inRHS);
    constexpr void operator-=(const T& inRHS);
    constexpr void operator*=(const T& inRHS);
    constexpr void operator/=(const T& inRHS);
    constexpr Vec<T, N> operator-() const;

protected:
    std::array<T, N> mComponents;
};
//

template <typename T, std::size_t N>
inline std::ostream& operator<<(std::ostream& inLHS, const Vec<T, N>& inRHS);

template <typename T>
using Vec2 = Vec<T, 2>;
using Vec2b = Vec2<bool>;
using Vec2i = Vec2<int32_t>;
using Vec2u = Vec2<uint32_t>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

template <typename T>
using Vec3 = Vec<T, 3>;
using Vec3b = Vec3<bool>;
using Vec3i = Vec3<int32_t>;
using Vec3u = Vec3<uint32_t>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template <typename T>
using Vec4 = Vec<T, 4>;
using Vec4b = Vec4<bool>;
using Vec4i = Vec4<int32_t>;
using Vec4u = Vec4<uint32_t>;
using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
}

#include "Vec.tcc"