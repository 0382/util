/**
 * @author:      0.382
 * @description: 自定义的一些方便输出输出的函数
 */

#pragma once
#ifndef EASY_PRINT_HPP
#define EASY_PRINT_HPP

#include <cstdlib>
#include <iostream>
#include <string_view>

namespace util
{

// 普通打印
template <typename... Args> void print(Args &&... args);
template <typename T> void print(T &&value)
{
    std::cout << value;
}
template <typename... Args> void print(Args &&... args)
{
    (print(args), ...);
}
template <typename... Args> void println(Args &&... args)
{
    (print(args), ...);
    std::cout << std::endl;
}

// 打印一些信息并终止程序
template <typename... Args> void stop(Args &&... args)
{
    println(args...);
    std::exit(-1);
}

// 带分隔符的打印
template <typename T, typename... Args> void sep_print(std::string_view sep, T &&val, Args &&... args);

template <typename T> void sep_print(std::string_view sep, T &&val)
{
    print(val);
}

template <typename T, typename... Args> void sep_print(std::string_view sep, T &&val, Args &&... args)
{
    print(val, sep);
    sep_print(sep, args...);
}

template <typename... Args> void sep_println(std::string_view sep, Args &&... args)
{
    sep_print(sep, args...);
    std::cout << std::endl;
}

} // namespace util

#endif // EASY_PRINT_HPP
