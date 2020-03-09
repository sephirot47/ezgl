#pragma once

#include <array>

// Returns a constexpr array repeating the passed value (see Repeat)
template <typename T, std::size_t N, std::size_t Index, typename... TArgs, typename = std::enable_if_t<(Index == N)>>
constexpr std::array<T, N> _GetArrayWithRepeatedValueRec(TArgs&&... inValues)
{
    return std::array<T, N> { std::forward<TArgs>(inValues)... };
}

template <typename T, std::size_t N, std::size_t Index, typename... TArgs, typename = std::enable_if_t<(Index < N)>>
constexpr std::array<T, N> _GetArrayWithRepeatedValueRec(const T& inValue, TArgs&&... inValues)
{
    return _GetArrayWithRepeatedValueRec<T, N, Index + 1>(inValue, inValue, std::forward<TArgs>(inValues)...);
}

template <typename T, std::size_t N>
constexpr std::array<T, N> GetArrayWithRepeatedValue(const T& inValue)
{
    return _GetArrayWithRepeatedValueRec<T, N, 2>(inValue, inValue);
}