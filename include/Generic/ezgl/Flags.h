#pragma once

#include <type_traits>

namespace ez
{

#define DECLARE_FLAGS(TFlagBits)                                                                                       \
  inline TFlagBits operator&(const TFlagBits inLHS, const TFlagBits inRHS)                                             \
  {                                                                                                                    \
    return static_cast<TFlagBits>(static_cast<std::underlying_type_t<TFlagBits>>(inLHS)                                \
        & static_cast<std::underlying_type_t<TFlagBits>>(inRHS));                                                      \
  }                                                                                                                    \
                                                                                                                       \
  inline TFlagBits operator|(const TFlagBits inLHS, const TFlagBits inRHS)                                             \
  {                                                                                                                    \
    return static_cast<TFlagBits>(static_cast<std::underlying_type_t<TFlagBits>>(inLHS)                                \
        | static_cast<std::underlying_type_t<TFlagBits>>(inRHS));                                                      \
  }                                                                                                                    \
                                                                                                                       \
  inline TFlagBits operator~(const TFlagBits inRHS)                                                                    \
  {                                                                                                                    \
    return static_cast<TFlagBits>(~static_cast<std::underlying_type_t<TFlagBits>>(inRHS));                             \
  }
}