#pragma once

#include "MathInitializers.h"

namespace egl
{
template <typename T>
constexpr auto Abs(const T& inValue)
{
  if constexpr (IsNumber_v<T>)
  {
    return std::abs(inValue);
  }
  else
  {
    auto result = inValue;
    for (auto& v : result) { v = Abs(v); }
    return result;
  }
}

template <typename T>
constexpr T Clamp(const T& inValue, const T& inMin, const T& inMax)
{
  if (inValue < inMin)
    return inMin;
  if (inValue > inMax)
    return inMax;
  return inValue;
}

template <typename T>
constexpr T Sign(const T& inValue)
{
  return (inValue < 0 ? static_cast<T>(-1) : static_cast<T>(1));
}

template <typename T>
constexpr bool Between(const T& inValue, const T& inMin, const T& inMax)
{
  return (inValue >= inMin) && (inValue <= inMax);
}

template <typename T, typename TInterpolationFactor>
constexpr T Lerp(const T& inFrom, const T& inTo, const TInterpolationFactor& t)
{
  return inFrom + (inTo - inFrom) * t;
}

template <typename T>
constexpr T DegreeToRad(const T& inV)
{
  constexpr T Factor = (Pi<T>() / 180.0);
  return inV * Factor;
}

template <typename T>
constexpr T RadToDegree(const T& inV)
{
  constexpr T Factor = (180.0 / Pi<T>());
  return inV * Factor;
}

}