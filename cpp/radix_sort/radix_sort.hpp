#pragma once
#ifndef UTIL_RADIX_SORT_HPP
#define UTIL_RADIX_SORT_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace util
{

template <unsigned item_bytes, unsigned compare_bytes>
void radix_sort_impl(std::size_t n, void *arr, void *buf)
{
    static_assert(compare_bytes <= item_bytes);
    using value_t = std::array<unsigned char, item_bytes>;
    using compare_t = std::array<unsigned char, compare_bytes>;
    constexpr unsigned bits = CHAR_BIT;
    constexpr unsigned bins = 1 << bits;
    constexpr unsigned mask = bins - 1;
    std::size_t count[compare_bytes][bins] = {0};

    value_t *x = static_cast<value_t *>(arr);
    value_t *t = static_cast<value_t *>(buf);
    for (std::size_t i = 0; i < n; ++i)
    {
        compare_t *xi = reinterpret_cast<compare_t *>(x + i);
        for (unsigned bit_idx = 0; bit_idx < compare_bytes; ++bit_idx)
        {
            ++count[bit_idx][(*xi)[bit_idx]];
        }
    }
    for (unsigned b = 1; b < bins; ++b)
    {
        for (unsigned bit_idx = 0; bit_idx < compare_bytes; ++bit_idx)
        {
            count[bit_idx][b] += count[bit_idx][b - 1];
        }
    }
    constexpr std::size_t npos = std::size_t(-1);
    for (unsigned bit_idx = 0; bit_idx < compare_bytes; ++bit_idx)
    {
        for (std::size_t i = n - 1; i != npos; --i)
        {
            compare_t *xi = reinterpret_cast<compare_t *>(x + i);
            t[--count[bit_idx][(*xi)[bit_idx]]] = x[i];
        }
        std::swap(x, t);
    }
    if (x != arr)
    {
        std::memcpy(arr, x, n * item_bytes);
    }
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void radix_sort_integer(std::size_t n, T *x)
{
    T *buf = new T[n];
    radix_sort_impl<sizeof(T), sizeof(T)>(n, static_cast<void *>(x), static_cast<void *>(buf));
    if constexpr (std::is_signed_v<T>)
    {
        if (x[0] < 0 || x[n - 1] >= 0)
        {
            goto end;
        }
        std::size_t start = 0;
        std::size_t stop = n - 1;
        std::size_t middle = (start + stop) / 2;
        do
        {
            if (x[middle] >= 0)
            {
                start = middle + 1;
            }
            else
            {
                stop = middle;
            }
            middle = (start + stop) / 2;
        }
        while (start < stop);
        std::memcpy(buf, x + middle, (n - middle) * sizeof(T));
        std::memcpy(buf + n - middle, x, middle * sizeof(T));
        std::memcpy(x, buf, n * sizeof(T));
    }
end:
    delete[] buf;
}

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

// template<> struct same_integer<long double>
// {
//     using type = __int128_t;
// };

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void radix_sort_float(std::size_t n, T *x)
{
    using same_int_t = typename same_integer<T>::type;
    static_assert(sizeof(T) == sizeof(same_int_t));
    same_int_t *buf = new same_int_t[n];
    radix_sort_impl<sizeof(T), sizeof(T)>(n, static_cast<void *>(x), static_cast<void *>(buf));
    same_int_t *px = reinterpret_cast<same_int_t *>(x);
    if (px[0] < 0) // all negative, reverse the array
    {
        std::reverse(x, x + n);
    }
    else if (px[n - 1] >= 0) // all positive, do nothing
    {}
    else
    {
        std::size_t pos = 0, rev_pos = n - 1;
        while (px[rev_pos] < 0)
        {
            buf[pos] = px[rev_pos];
            ++pos;
            --rev_pos;
        }
        std::memcpy(buf + pos, px, (rev_pos + 1) * sizeof(T));
        std::memcpy(px, buf, n * sizeof(T));
    }
    delete[] buf;
}

template <typename T>
void radix_sort(std::size_t n, T *x)
{
    if constexpr (std::is_integral_v<T>)
    {
        radix_sort_integer<T>(n, x);
    }
    else if (std::is_floating_point_v<T>)
    {
        radix_sort_float<T>(n, x);
    }
    else
    {
        T *buf = new T[n];
        radix_sort_impl<sizeof(T), sizeof(T)>(n, static_cast<void *>(x), static_cast<void *>(buf));
        delete[] buf;
    }
}

} // end namespace util

#endif // UTIL_RADIX_SORT_HPP