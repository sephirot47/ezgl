#include "Vec.h"

namespace egl
{

template <typename T, std::size_t N>
constexpr Vec<T, N>::Vec(const T& inAllValue) noexcept
    : mComponents{ GetArrayWithRepeatedValue<T, N>(inAllValue) }
{
}

template <typename T, std::size_t N>
template <typename... TArgs, typename>
constexpr Vec<T, N>::Vec(TArgs&&... inArgs) noexcept
    : mComponents{ inArgs... }
{
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::Dot(const Vec& inLHS, const Vec& inRHS)
{
    T dot{ 0 };
    for (std::size_t i = 0; i < N; ++i)
    {
        dot += inLHS[i] * inRHS[i];
    }
    return dot;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::Zero()
{
    return Vec(0);
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::One()
{
    return Vec(1);
}

template <typename T, std::size_t N>
T& Vec<T, N>::operator[](std::size_t i)
{
    return mComponents[i];
}

template <typename T, std::size_t N>
constexpr const T& Vec<T, N>::operator[](std::size_t i) const
{
    return mComponents[i];
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator+(const Vec<T, N>& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] + inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator-(const Vec& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] * inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator*(const Vec& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] * inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator/(const Vec& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator+(const T& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator-(const T& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator*(const T& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator/(const T& inRHS) const
{
    Vec<T, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
inline std::ostream& operator<<(std::ostream& inLHS, const Vec<T, N>& inRHS)
{
    inLHS << "(";
    for (auto i = 0; i < N; ++i)
        inLHS << inRHS[i] << (i < N - 1 ? ", " : "");
    inLHS << ")";
    return inLHS;
}
}
