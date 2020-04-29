#pragma once

namespace egl
{
template <typename T>
struct MITAll final
{
  T mAllValue;
};

struct MITAdditiveIdentity final
{
};

struct MITMultiplicativeIdentity final
{
};
}