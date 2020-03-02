#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <iostream>
#include <initializer_list>

namespace egl
{
template <typename T>
;
struct All
{
        All((const T &inV) : v(inV) {} 
        T v;
}

template <typename T, uint8_t N>
class Vec
{
public:
    using ValueType = T;
    static constexpr auto NumComponents = N;

    Vec() = default;

    template <typename... TArgs>
    Vec(TArgs&&... inArgs)
    {
        const auto inArgsArray = { inArgs... };
        int i = 0;
        for (const auto& v : inArgsArray)
        {
            mComponents[i] = v;
            ++i;
        }
    }

    Vec(const All& inValueForAllComponents)
    {
        for (int i = 0; i < N; ++i)
            mComponents[i] = inValueForAllComponents;
    }

    Vec(const Vec&) = default;
    Vec& operator=(const Vec&) = default;

    Vec(Vec&&) = default;
    Vec& operator=(Vec&&) = default;

    ~Vec() = default;

    T& Get(std::size_t i);
    const T& Get(std::size_t i) const;
    T& operator[](std::size_t i);
    const T& operator[](std::size_t i) const;

    static const Vec& One()
    {
        static const Vec one;
        for (int i = 0; i < N; ++i)
            one[i] = 1;
        return one;
    }

protected:
    std::array<T, N> mComponents;
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