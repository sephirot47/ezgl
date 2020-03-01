#include "Vec.h"

namespace egl
{
template <typename T, uint8_t N, typename TVec>
T& _VecBase<T, N, TVec>::Get(std::size_t i)
{
    return ((T*)(this))[i];
}

template <typename T, uint8_t N, typename TVec>
const T& _VecBase<T, N, TVec>::Get(std::size_t i) const
{
    return ((const T*)(this))[i];
}

template <typename T, uint8_t N, typename TVec>
T& _VecBase<T, N, TVec>::operator[](std::size_t i)
{
    return Get(i);
}

template <typename T, uint8_t N, typename TVec>
const T& _VecBase<T, N, TVec>::operator[](std::size_t i) const
{
    return Get(i);
}
}