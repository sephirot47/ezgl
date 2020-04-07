#pragma once

#include <type_traits>

namespace egl
{
template <typename T>
constexpr auto IsNumber_v = std::is_arithmetic_v<T>;

template <typename T>
constexpr auto IsVecOrMat_v = IsVec_v<T> || IsMat_v<T>;
}
