#pragma once
#ifndef UTIL_RADIX_SORT_HPP
#define UTIL_RADIX_SORT_HPP

#include <algorithm>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>


namespace util
{

template <typename T>
struct radix_trait
{
    static constexpr unsigned radix_bytes = sizeof(T) / sizeof(unsigned char);
    template <unsigned index>
    static constexpr unsigned char get(T x) noexcept
    {
        static_assert(index < radix_bytes);
        if constexpr (std::is_integral_v<T> && std::is_signed_v<T> && (index == radix_bytes - 1))
        {
            return ((unsigned char *)&x)[index] ^ 0x80;
        }
        else
        {
            return ((unsigned char *)&x)[index];
        }
    }
};

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
struct same_integer
{
    using type = int;
};

template <>
struct same_integer<float>
{
    using type = int32_t;
};

template <>
struct same_integer<double>
{
    using type = int64_t;
};

template <unsigned byte_idx, typename T, typename Trait = radix_trait<T>>
void radix_sort_impl(std::size_t n, T *arr, T *buf)
{
    constexpr unsigned radix_bytes = Trait::radix_bytes;
    static_assert(byte_idx < radix_bytes);
    constexpr unsigned bits = std::numeric_limits<unsigned char>::digits;
    constexpr unsigned bins = 1 << bits;
    std::size_t count[bins] = {0};
    for (std::size_t i = 0; i < n; ++i)
    {
        ++count[Trait::template get<byte_idx>(arr[i])];
    }
    for (unsigned b = 1; b < bins; ++b)
    {
        count[b] += count[b - 1];
    }
    constexpr std::size_t npos = std::size_t(-1);
    for (std::size_t i = n - 1; i != npos; --i)
    {
        buf[--count[Trait::template get<byte_idx>(arr[i])]] = arr[i];
    }
    if constexpr (byte_idx < radix_bytes - 1)
    {
        radix_sort_impl<unsigned(byte_idx + 1), T, Trait>(n, buf, arr);
    }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void radix_sort_float(std::size_t n, T *arr, T *buf)
{
    using same_int_t = typename same_integer<T>::type;
    static_assert(sizeof(T) == sizeof(same_int_t));
    same_int_t *parr = reinterpret_cast<same_int_t *>(arr);
    same_int_t *pbuf = reinterpret_cast<same_int_t *>(buf);
    using same_uint_t = std::make_unsigned_t<same_int_t>;
    radix_sort_impl<0>(n, (same_uint_t *)arr, (same_uint_t *)buf);
    if (parr[0] < 0) // all negative, reverse the array
    {
        std::reverse(arr, arr + n);
    }
    else if (parr[n - 1] >= 0) // all positive, do nothing
    {}
    else
    {
        std::size_t pos = 0, rev_pos = n - 1;
        while (parr[rev_pos] < 0)
        {
            pbuf[pos] = parr[rev_pos];
            ++pos;
            --rev_pos;
        }
        std::memcpy(pbuf + pos, parr, (rev_pos + 1) * sizeof(T));
        std::memcpy(parr, pbuf, n * sizeof(T));
    }
}

template <typename T>
void radix_sort(std::size_t n, T *arr, T *buf = nullptr)
{
    std::unique_ptr<unsigned char[]> resource;
    if (buf == nullptr)
    {
        resource = std::make_unique<unsigned char[]>(n * radix_trait<T>::radix_bytes);
        buf = (T *)resource.get();
    }
    if constexpr (std::is_floating_point_v<T>)
    {
        radix_sort_float<T>(n, arr, buf);
    }
    else
    {
        radix_sort_impl<0>(n, arr, buf);
    }
}

} // end namespace util

#endif // UTIL_RADIX_SORT_HPP