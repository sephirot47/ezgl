#include "Vec.h"

namespace egl
{
template <typename T, uint8_t N>
T& Vec<T, N>::Get(std::size_t i)
{
    return ((T*)(this))[i];
}

template <typename T, uint8_t N>
const T& Vec<T, N>::Get(std::size_t i) const
{
    return ((const T*)(this))[i];
}

template <typename T, uint8_t N>
T& Vec<T, N>::operator[](std::size_t i)
{
    return Get(i);
}

template <typename T, uint8_t N>
const T& Vec<T, N>::operator[](std::size_t i) const
{
    return Get(i);
}
}