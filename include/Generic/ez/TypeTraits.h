#pragma once

namespace ez
{
// IsLight.
template <typename T>
struct IsLight final : std::false_type
{
};
template <typename T>
constexpr bool IsLight_v = IsLight<T>::value;
}