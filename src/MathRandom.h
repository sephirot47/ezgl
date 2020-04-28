#pragma once

#include "MathInitializers.h"

namespace egl
{
template <typename T>
constexpr auto RandomUnit()
{
  if constexpr (IsNumber_v<T>)
  {
    return static_cast<T>(static_cast<double>(rand()) / RAND_MAX);
  }
  else if constexpr (IsQuat_v<T>)
  {
    using ValueType = typename T::ValueType;
    return AngleAxis(RandomUnit<ValueType>() * FullCircleRads<ValueType>(), RandomUnit<Vec3<ValueType>>());
  }
  else
  {
    using ValueType = typename T::ValueType;

    T result;
    for (auto& value : result) { value = RandomUnit<ValueType>(); }
    result = Normalized(result);
    return result;
  }
}

template <typename T>
// Min always included. Max excluded in integer types, included in real types.
constexpr auto Random(const T& inMin, const T& inMax)
{
  if constexpr (IsNumber_v<T>)
  {
    return static_cast<T>(RandomUnit<double>() * (inMax - inMin) + inMin);
  }
  else
  {
    using ValueType = typename T::ValueType;

    T result;
    auto min_it = inMin.begin();
    auto max_it = inMax.begin();
    auto result_it = result.begin();
    while (result_it != result.end())
    {
      *result_it = Random<ValueType>(*min_it, *max_it);
      ++min_it;
      ++max_it;
      ++result_it;
    }
    return result;
  }
}

template <typename T>
constexpr auto RandomSign()
{
  if constexpr (IsNumber_v<T>)
  {
    return (rand() % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
  }
  else
  {
    using ValueType = typename T::ValueType;
    T result;
    for (auto& value : result) value = RandomSign<ValueType>();
    return result;
  }
}
}