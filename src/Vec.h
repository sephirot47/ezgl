#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <iostream>

namespace egl
{

// Bases and specializations to have x, y, z, w
template <typename T, uint8_t N, typename TVec>
class _VecBase
{
public:
    using ValueType = T;
    static constexpr auto NumComponents = N;

    static const TVec& One()
    {
        static TVec one;
        for (int i = 0; i < N; ++i)
            one[i] = 1;
        return one;
    }

    T& Get(std::size_t i);
    const T& Get(std::size_t i) const;
    T& operator[](std::size_t i);
    const T& operator[](std::size_t i) const;

protected:
    _VecBase() = default;
    _VecBase(const _VecBase&) = default;
    _VecBase& operator=(const _VecBase&) = default;
    _VecBase(_VecBase&&) = default;
    _VecBase& operator=(_VecBase&&) = default;
    ~_VecBase() = default;
};

template <typename T, uint8_t N>
class Vec : public _VecBase<T, N, Vec<T, N> >
{
public:
    using Base = _VecBase<T, N, Vec<T, N> >;
    using ValueType = T;
    static constexpr auto NumComponents = N;

protected:
    std::array<T, N> mComponents;
};

template <typename T>
class Vec<T, 1u> : public _VecBase<T, 1u, Vec<T, 1u> >
{
public:
    using Base = _VecBase<T, 1u, Vec<T, 1u> >;
    using ValueType = T;
    T x;
};

template <typename T>
class Vec<T, 2u> : public _VecBase<T, 2u, Vec<T, 2u> >
{
public:
    using Base = _VecBase<T, 2u, Vec<T, 2u> >;
    using ValueType = T;
    T x, y;
};

template <typename T>
class Vec<T, 3u> : public _VecBase<T, 3u, Vec<T, 3u> >
{
public:
    using Base = _VecBase<T, 3u, Vec<T, 3u> >;
    using ValueType = T;
    T x, y, z;
};

template <typename T>
class Vec<T, 4u> : public _VecBase<T, 4u, Vec<T, 4u> >
{
public:
    using Base = _VecBase<T, 4u, Vec<T, 4u> >;
    using ValueType = T;
    T x, y, z, w;
};
//

template <typename T, uint8_t N>
inline std::ostream& operator<<(std::ostream& inLHS, const Vec<T, N>& inRHS)
{
    inLHS << "(";
    for (auto i = 0; i < N; ++i)
        inLHS << inRHS.Get(i) << (i < N - 1 ? ", " : "");
    inLHS << ")";
    return inLHS;
}

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