/**
 * @author:      0.382
 * @description: 插值法
 */

#pragma once
#ifndef UTIL_INTERPOLATION_HPP
#define UTIL_INTERPOLATION_HPP

#include "mymath.hpp"

namespace util
{

namespace detail
{

template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
T Lagrange_interpolation(T x, const T *Xn, const T *Yn, std::size_t size)
{
    T result = 0;
    for (std::size_t i = 0; i < size; ++i)
    {
        T L = 1;
        for (std::size_t j = 0; j < size; ++j)
        {
            if (j == i)
                continue;
            L *= (x - Xn[j]) / (Xn[i] - Xn[j]); // 2*N^2 次减法，2*N^2次乘除法
        }
        result += L * Yn[i];
    }
    return result;
}

// 这种写法其实没有体现出 Newton 算法可以随意新增支撑点的特性
// 此外，我们还可以看到，a 系数实际是不依赖于插值点的
// 也就是说如果要计算多个插值点，a 系数是可以重复利用的
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
T Newton_interpolation(T x, const T *Xn, const T *Yn, std::size_t size)
{
    T result = 0, N = 1;
    T *a = new T[size]; // 额外的 N * sizeof(T) 内存
    for (std::size_t i = 0; i < size; ++i)
    {
        T M = 1;
        a[i] = Yn[i];
        for (std::size_t j = 0; j < i; ++j)
        {
            a[i] -= M * a[j];
            M *= Xn[i] - Xn[j]; // N^2次乘法，N^2次减法
        }
        a[i] /= M;
        result += a[i] * N;
        N *= (x - Xn[i]);
    }
    delete[] a;
    return result;
}

template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
T Neville_interpolation(T x, const T *Xn, const T *Yn, std::size_t size)
{
    T *t = new T[size]; // 额外的 N * sizeof(T) 内存
    for (std::size_t i = 0; i < size; ++i)
    {
        t[i] = Yn[i];
    }
    for (std::size_t k = 1; k < size; ++k)
    {
        for (std::size_t i = 0; i < size - k; ++i)
        {
            // N^2 次除法，5/2 * N^2 次加减法
            t[i] = t[i + 1] + (t[i + 1] - t[i]) / ((x - Xn[i]) / (x - Xn[i + k]) - 1);
        }
    }
    T result = t[0];
    delete[] t;
    return result;
}

} // end namespace detail

enum class interp
{
    Lagrange,
    Newton,
    Neville
};

// 通用的多项式插值模板
// x: 插值点
// Xn: 已有的自变量数据
// Yn: 已有的函数数据
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
T interpolation(T x, const T *Xn, const T *Yn, std::size_t N, interp _type = interp::Lagrange)
{
    switch (_type)
    {
    case interp::Lagrange: return detail::Lagrange_interpolation<T>(x, Xn, Yn, N);
    case interp::Newton: return detail::Newton_interpolation<T>(x, Xn, Yn, N);
    case interp::Neville: return detail::Neville_interpolation<T>(x, Xn, Yn, N);
    default: return std::nan("");
    }
}

} // end namespace util

#endif // UTIL_INTERPOLATION_HPP