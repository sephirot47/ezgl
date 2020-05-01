#pragma once

#include "ezgl/Vec.h"
#include <cstdint>

namespace egl
{
template <typename T, ::std::size_t N>
using Color = Vec<T, N>;

template <typename T>
using Color3 = Color<T, 3>;
using Color3f = Color3<float>;
using Color3d = Color3<double>;

template <typename T>
using Color4 = Color<T, 4>;
using Color4f = Color4<float>;
using Color4d = Color4<double>;
}