/**
 * @author:      0.382
 * @description: 尝试将字符串转数字类型
 */

#pragma once
#ifndef TRYPARSE_HPP
#define TRYPARSE_HPP

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <optional>
#include <string>
#include <type_traits>

namespace util
{

template <typename T>
std::optional<std::enable_if_t<std::is_integral_v<T>, T>> _tryparse_integer(const std::string &str, int base)
{
    char *p;
    T value;
    if constexpr (std::is_same_v<T, long long>)
        value = std::strtoll(str.c_str(), &p, base);
    else if (std::is_same_v<T, unsigned long long>)
        value = std::strtoull(str.c_str(), &p, base);
    else if (std::is_signed_v<T>)
        value = std::strtol(str.c_str(), &p, base);
    else if (std::is_unsigned_v<T>)
        value = std::strtoul(str.c_str(), &p, base);
    else
        return std::nullopt;
    if (errno == ERANGE)
    {
        errno = 0;
        return std::nullopt;
    }
    if (!std::all_of(static_cast<const char *>(p), str.c_str() + str.size(), static_cast<int (*)(int)>(std::isspace)))
        return std::nullopt;
    return value;
}

template <typename T>
std::optional<std::enable_if_t<std::is_floating_point_v<T>, T>> _tryparse_real(const std::string &str)
{
    char *p;
    T value;
    if constexpr (std::is_same_v<T, float>)
        value = std::strtof(str.c_str(), &p);
    else if (std::is_same_v<T, double>)
        value = std::strtod(str.c_str(), &p);
    else if (std::is_same_v<T, long double>)
        value = std::strtold(str.c_str(), &p);
    else
        return std::nullopt;
    if (errno == ERANGE)
    {
        errno = 0;
        return std::nullopt;
    }
    if (!std::all_of(static_cast<const char *>(p), str.c_str() + str.size(), static_cast<int (*)(int)>(std::isspace)))
        return std::nullopt;
    return value;
}

template <typename T>
std::optional<std::enable_if_t<std::is_arithmetic_v<T>, T>> tryparse(const std::string &str)
{
    if constexpr (std::is_integral_v<T>)
        return _tryparse_integer<T>(str, 10);
    else
        return _tryparse_real<T>(str);
}

template <typename T>
std::optional<std::enable_if_t<std::is_integral_v<T>, T>> tryparse(const std::string &str, int base)
{
    return _tryparse_integer<T>(str, base);
}

} // end namespace util

#endif // TRYPARSE_HPP
