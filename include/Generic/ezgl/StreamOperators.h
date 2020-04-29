#pragma once

#include <experimental/type_traits>
#include <filesystem>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace egl
{
template <typename TFirst, typename TSecond>
std::ostream& operator<<(std::ostream& ioLHS, const std::pair<TFirst, TSecond>& inRHS)
{
  ioLHS << "<" << inRHS.first << ", " << inRHS.second << ">";
  return ioLHS;
}

template <typename T>
std::ostream& operator<<(std::ostream& ioLHS, const std::optional<T>& inRHS)
{
  ioLHS << "OPT<";
  if (inRHS.has_value())
    ioLHS << *inRHS;
  else
    ioLHS << "null";
  ioLHS << ">";
  return ioLHS;
}

// clang-format off
template <typename T, typename = void>
struct HadAlreadyOstreamOperator : std::false_type {};

template <typename T>
struct HadAlreadyOstreamOperator<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>> : std::true_type {};

template <typename T>
constexpr bool HadAlreadyOstreamOperator_v = HadAlreadyOstreamOperator<T>::value;
// clang-format on

template <typename T,
    typename = decltype(std::declval<T>().cbegin()),              // Has cbegin
    typename = std::enable_if_t<!HadAlreadyOstreamOperator_v<T>>> // Was not overloaded before
std::ostream& operator<<(std::ostream& ioLHS, const T& inRHS)
{
  ioLHS << "(";
  for (auto it = inRHS.cbegin(); it != inRHS.cend(); ++it) ioLHS << (it == inRHS.cbegin() ? "" : ", ") << (*it);
  ioLHS << ")";
  return ioLHS;
}
}