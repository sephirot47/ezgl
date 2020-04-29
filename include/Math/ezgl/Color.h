#pragma once

#include "ezgl/Vec.h"

namespace egl
{
template <typename T>
using Color3 = Vec<T, 3>;
using Color3f = Color3<float>;
using Color3d = Color3<double>;

template <typename T>
using Color4 = Vec<T, 4>;
using Color4f = Color4<float>;
using Color4d = Color4<double>;
}