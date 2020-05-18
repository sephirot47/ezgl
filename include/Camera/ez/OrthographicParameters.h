#pragma once

#include "ez/MathInitializers.h"
#include "ez/Vec.h"

namespace ez
{
template <typename T, std::size_t N>
struct OrthographicParameters final
{
  Vec<T, N> mMin = Zero<Vec<T, N>>();
  Vec<T, N> mMax = One<Vec<T, N>>();
};

template <typename T>
using OrthographicParameters2 = OrthographicParameters<T, 2>;
using OrthographicParameters2f = OrthographicParameters2<float>;

template <typename T>
using OrthographicParameters3 = OrthographicParameters<T, 3>;
using OrthographicParameters3f = OrthographicParameters3<float>;
}