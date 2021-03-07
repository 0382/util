/**
 * @author:      0.382
 * @description: 数值积分
 */

#pragma once
#ifndef UTIL_INTEGRAL_HPP
#define UTIL_INTEGRAL_HPP

#include "mymath.hpp"
#include <functional>

namespace util
{

/*  N   σ_i                   Ns     名称
 *  1   1,1                        2      梯形法则
 *  2   1,4,1                      6      Simpson规则
 *  3   1,3,3,1                    8      3/8规则
 *  4   7,32,12,32,7               90     Milne规则
 *  5   19,75,50,50,75,19          288
 *  6   41,216,27,272,27,216,41    840    Weddle规则
 */

enum class IntLevel
{
    lv1 = 1,
    lv2,
    lv3,
    lv4,
    lv5,
    lv6,
    trapezoid = lv1,
    Simpson = lv2,
    Milne = lv4,
    Weddle = lv6
};

// 积分选项
// 一个是积分取的点数目，默认是10000个点
// 一个是使用的积分近似方法，默认是梯形法则
struct IntOptions
{
    IntLevel level;
    int Npoints;
    IntOptions() : level(IntLevel::lv1), Npoints(10000) {}
    IntOptions(IntLevel _level, int _Npoints) : level(_level), Npoints(_Npoints) {}
    IntOptions(IntLevel _level) : IntOptions(_level, 10000) {}
    IntOptions(int _Npoints) : IntOptions(IntLevel::lv1, _Npoints) {}
};

namespace detail
{

template <typename value_type>
value_type integral_lv1(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    value_type h = (b - a) / N;
    value_type result = (f(a) + f(b)) / 2;
    value_type x = a + h;
    for (int i = 1; i < N; ++i, x += h)
    {
        result += f(x);
    }
    return result * std::abs(h);
}

template <typename value_type>
value_type integral_lv2(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    N = isodd(N) ? N + 1 : N;
    value_type h = (b - a) / N;
    value_type result = f(b) - f(a);
    value_type x = a;
    for (int i = 0; i < N / 2; ++i)
    {
        result += 2 * f(x);
        x += h;
        result += 4 * f(x);
        x += h;
    }
    return result * std::abs(h) / 3;
}

template <typename value_type>
value_type integral_lv3(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    N = (N / 3) * 3 + 3;
    value_type h = (b - a) / N;
    value_type result = f(b) - f(a);
    value_type x = a;
    for (int i = 0; i < N / 3; ++i)
    {
        result += 2 * f(x);
        x += h;
        result += 3 * f(x);
        x += h;
        result += 3 * f(x);
        x += h;
    }
    return result * std::abs(h) / (8. / 3.);
}

template <typename value_type>
value_type integral_lv4(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    N = (N / 4) * 4 + 4;
    value_type h = (b - a) / N;
    value_type result = 7 * (f(b) - f(a));
    value_type x = a;
    for (int i = 0; i < N / 4; ++i)
    {
        result += 14 * f(x);
        x += h;
        result += 32 * f(x);
        x += h;
        result += 12 * f(x);
        x += h;
        result += 32 * f(x);
        x += h;
    }
    return result * std::abs(h) / (90. / 4.);
}

template <typename value_type>
value_type integral_lv5(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    N = (N / 5) * 5 + 5;
    value_type h = (b - a) / N;
    value_type result = 19 * (f(b) - f(a));
    value_type x = a;
    for (int i = 0; i < N / 5; ++i)
    {
        result += 38 * f(x);
        x += h;
        result += 75 * f(x);
        x += h;
        result += 50 * f(x);
        x += h;
        result += 50 * f(x);
        x += h;
        result += 75 * f(x);
        x += h;
    }
    return result * std::abs(h) / (288. / 5.);
}

template <typename value_type>
value_type integral_lv6(value_type a, value_type b, std::function<value_type(value_type)> f, int N)
{
    N = (N / 6) * 6 + 6;
    value_type h = (b - a) / N;
    value_type result = 41 * (f(b) - f(a));
    value_type x = a;
    for (int i = 0; i < N / 6; ++i)
    {
        result += 82 * f(x);
        x += h;
        result += 216 * f(x);
        x += h;
        result += 27 * f(x);
        x += h;
        result += 272 * f(x);
        x += h;
        result += 27 * f(x);
        x += h;
        result += 216 * f(x);
        x += h;
    }
    return result * std::abs(h) / 140;
}

} // end namespace detail

// 通用积分函数模板
// a, b 为积分区间
// f为积分函数
template <typename value_type = double, typename = typename std::enable_if_t<std::is_floating_point_v<value_type>>>
value_type integral(value_type a, value_type b, std::function<value_type(value_type)> f,
                    const IntOptions &opts = IntOptions())
{
    if (opts.level == IntLevel::lv1)
        return detail::integral_lv1(a, b, f, opts.Npoints);
    else if (opts.level == IntLevel::lv2)
        return detail::integral_lv2(a, b, f, opts.Npoints);
    else if (opts.level == IntLevel::lv3)
        return detail::integral_lv3(a, b, f, opts.Npoints);
    else if (opts.level == IntLevel::lv4)
        return detail::integral_lv4(a, b, f, opts.Npoints);
    else if (opts.level == IntLevel::lv5)
        return detail::integral_lv5(a, b, f, opts.Npoints);
    else if (opts.level == IntLevel::lv6)
        return detail::integral_lv6(a, b, f, opts.Npoints);
    else
        return std::nan("");
}

} // end namespace util

#endif // UTIL_INTEGRAL_HPP