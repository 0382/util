/**
 * @author:      0.382
 * @description: 一些有用的数学函数
 */

#pragma once
#ifndef UTIL_MYMATH_HPP
#define UTIL_MYMATH_HPP

#include <cmath>
#include <complex>
#include <type_traits>

namespace util
{

//判断类型是否为复数类型
template <typename T>
struct is_complex
{
    static constexpr bool value = false;
};

template <typename InnerT>
struct is_complex<std::complex<InnerT>>
{
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool is_complex_v = is_complex<T>::value;

// 判断是否是奇数
template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
inline bool isodd(T n)
{
    return n & 0x01;
}

// 判断是否是偶数
template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
inline bool iseven(T n)
{
    return !isodd(n);
}

// 返回当前浮点数与下一个浮点数的差
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
T eps(T value)
{
    if (value == static_cast<T>(0))
        return std::nextafter(static_cast<T>(0), static_cast<T>(1));
    else
        return std::abs(std::nextafter(value, 2 * value) - value);
}

// 简化版本的 julia 约等于算符
// 如果 atol 等于零，返回 abs(a - b) <= sqrt(max(eps(a), eps(b)))
// 否则，返回 abs(a - b) <= atol
// 特别的，如果 atol = 0, 则approx(x, 0) = (x == 0)
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
bool approx(T a, T b, T atol = 0)
{
    if (atol == 0)
    {
        if (a == 0 || b == 0)
            return a == b;
        else
            return std::abs(a - b) <= std::sqrt(std::max(eps(a), eps(b)));
    }
    else
    {
        return std::abs(a - b) <= atol;
    }
}

// 复数版本的约等于
// 实数的 abs 替换成复数的 abs
// 将实数的 eps 替换成
// 实部虚部分别 eps 求和
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
bool approx(std::complex<T> a, std::complex<T> b, T atol = 0)
{
    if (atol == 0)
    {
        if (a == 0 || b == 0)
            return a == b;
        else
        {
            auto eps_a = eps(std::real(a)) + eps(std::imag(a));
            auto eps_b = eps(std::real(b)) + eps(std::imag(b));
            return std::abs(a - b) <= std::sqrt(std::max(eps_a, eps_b));
        }
    }
    else
    {
        return std::abs(a - b) <= atol;
    }
}

} // end namespace util

#endif // UTIL_MYMATH_HPP