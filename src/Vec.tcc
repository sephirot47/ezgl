#include "Vec.h"

#include <cmath>

namespace egl
{

template <typename T, std::size_t N>
constexpr Vec<T, N>::Vec(const T& inAllValue) noexcept
    : mComponents { GetArrayWithRepeatedValue<T, N>(inAllValue) }
{
}

template <typename T, std::size_t N>
template <typename... TArgs, typename>
constexpr Vec<T, N>::Vec(TArgs&&... inArgs) noexcept
    : mComponents { std::forward<TArgs>(inArgs)... }
{
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::Length() const
{
    return std::sqrt(SqLength());
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::SqLength() const
{
    return Vec::Dot(*this, *this);
}

template <typename T, std::size_t N>
[[nodiscard]] constexpr Vec<T, N> Vec<T, N>::Normalized() const
{
    return *this / Length();
}

template <typename T, std::size_t N>
void Vec<T, N>::Normalize()
{
    *this = this->Normalized();
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::SqDistance(const Vec& inLHS, const Vec& inRHS)
{
    const auto diff = (inRHS - inLHS);
    return Vec::SqLength(diff, diff);
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::Distance(const Vec& inLHS, const Vec& inRHS)
{
    return std::sqrt(Vec::SqLength(inLHS, inRHS));
}

template <typename T, std::size_t N>
constexpr T Vec<T, N>::Dot(const Vec& inLHS, const Vec& inRHS)
{
    T dot { 0 };
    for (std::size_t i = 0; i < N; ++i)
    {
        dot += inLHS[i] * inRHS[i];
    }
    return dot;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::Cross(const Vec& inLHS, const Vec& inRHS)
{
    static_assert(N == 3 && "Cross product is only defined for Vec3.");
    return { inLHS[1] * inRHS[2] - inLHS[2] * inRHS[1],
        inLHS[2] * inRHS[0] - inLHS[0] * inRHS[2],
        inLHS[0] * inRHS[1] - inLHS[1] * inRHS[0] };
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::Zero()
{
    return All(0);
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::One()
{
    return All(1);
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::All(const T& inAllValue)
{
    return Vec(inAllValue);
}

template <typename T, std::size_t N>
T* Vec<T, N>::Data()
{
    return &mComponents[0];
}

template <typename T, std::size_t N>
const T* Vec<T, N>::Data() const
{
    return &mComponents[0];
}

template <typename T, std::size_t N>
constexpr std::array<T, N>::iterator Vec<T, N>::begin()
{
    return std::begin(mComponents);
}

template <typename T, std::size_t N>
constexpr std::array<T, N>::iterator Vec<T, N>::end()
{
    return std::end(mComponents);
}
template <typename T, std::size_t N>
constexpr std::array<T, N>::const_iterator Vec<T, N>::begin() const
{
    return std::begin(mComponents);
}

template <typename T, std::size_t N>
constexpr std::array<T, N>::const_iterator Vec<T, N>::end() const
{
    return std::end(mComponents);
}

template <typename T, std::size_t N>
constexpr std::array<T, N>::const_iterator Vec<T, N>::cbegin() const
{
    return std::cbegin(mComponents);
}

template <typename T, std::size_t N>
constexpr std::array<T, N>::const_iterator Vec<T, N>::cend() const
{
    return std::cend(mComponents);
}

template <typename T, std::size_t N>
constexpr T& Vec<T, N>::operator[](std::size_t i)
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
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] + inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator-(const Vec& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] - inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator*(const Vec& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] * inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator/(const Vec& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator+(const T& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] + inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator-(const T& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] - inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator*(const T& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] * inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator/(const T& inRHS) const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = mComponents[i] / inRHS;
    }
    return result;
}

template <typename T, std::size_t N>
constexpr Vec<T, N> Vec<T, N>::operator-() const
{
    Vec<T, N> result {};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = -mComponents[i];
    }
    return result;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator+=(const Vec<T, N>& inRHS)
{
    *this = *this + inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator-=(const Vec<T, N>& inRHS)
{
    *this = *this - inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator*=(const Vec<T, N>& inRHS)
{
    *this = *this * inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator/=(const Vec<T, N>& inRHS)
{
    *this = *this / inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator+=(const T& inRHS)
{
    *this = *this + inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator-=(const T& inRHS)
{
    *this = *this - inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator*=(const T& inRHS)
{
    *this = *this * inRHS;
}

template <typename T, std::size_t N>
constexpr void Vec<T, N>::operator/=(const T& inRHS)
{
    *this = *this / inRHS;
}

template <typename T, std::size_t N>
inline std::ostream& operator<<(std::ostream& inLHS, const Vec<T, N>& inRHS)
{
    inLHS << "(";
    for (std::size_t i = 0; i < N; ++i)
        inLHS << inRHS[i] << (i < N - 1 ? " " : "");
    inLHS << ")";
    return inLHS;
}
}
