#pragma once

#include <type_traits>

namespace ez
{
// IsLight.
template <typename T>
struct IsLight final : std::false_type
{
};
template <typename T>
constexpr bool IsLight_v = IsLight<std::remove_cvref_t<T>>::value;
}