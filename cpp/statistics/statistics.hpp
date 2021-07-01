/**
 * @author: 0.382
 * @description: some functions for statistics
 * @url: https://github.com/0382/util/tree/main/cpp/statistics
 */

#pragma once
#ifndef UTIL_STATISTICS_HPP
#define UTIL_STATISTICS_HPP

#include <cmath>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <vector>

namespace util
{

struct statistics_draft
{
    double sum;
    double mean;
    double variance;
    double stddev;

    friend std::ostream &operator<<(std::ostream &os, const statistics_draft &dft)
    {
        os << "sum = " << dft.sum << "\n";
        os << "mean = " << dft.mean << "\n";
        os << "variance = " << dft.variance << "\n";
        os << "stddev = " << dft.stddev << "\n";
        return os;
    }
};

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
T sum(const std::vector<T> &data)
{
    return std::accumulate(data.cbegin(), data.cend(), static_cast<T>(0));
}

template <typename InputIterator, typename T = std::remove_cv_t<std::remove_reference_t<decltype(*InputIterator())>>,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
T sum(InputIterator first, InputIterator last)
{
    return std::accumulate(first, last, static_cast<T>(0));
}

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
double mean(const std::vector<T> &data)
{
    if (data.empty())
    {
        return 0.;
    }
    return sum(data) / static_cast<double>(data.size());
}

template <typename InputIterator>
double mean(InputIterator first, InputIterator last)
{
    if (first == last)
    {
        return 0;
    }
    return sum(first, last) / static_cast<double>(last - first);
}

// variance(data, dof)
//   parameter:
//     data: a data vector
//     dof: degree of freedom,
//          for standard variance, use the default dof = 0,
//          for sample variance, use dof = 1
template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
double variance(const std::vector<T> &data, int dof = 0)
{
    if (data.empty() || data.size() <= dof)
    {
        return -1;
    }
    double xbar = mean(data);
    double addup =
        std::accumulate(data.cbegin(), data.cend(), 0., [&xbar](double a, T b) { return a + (b - xbar) * (b - xbar); });
    return addup / (data.size() - dof);
}

template <typename InputIterator>
double variance(InputIterator first, InputIterator last, int dof = 0)
{
    using T = std::remove_cv_t<std::remove_reference_t<decltype(*first)>>;
    static_assert(std::is_arithmetic_v<T>, "statistics only for arithmetic type");
    if (last - first <= dof)
    {
        return -1;
    }
    double xbar = mean(first, last);
    double addup = std::accumulate(first, last, 0., [&xbar](double a, T b) { return a + (b - xbar) * (b - xbar); });
    return addup / ((last - first) - dof);
}

// standard deviation, parameters are similar to variance
template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
double stddev(const std::vector<T> &data, int dof = 0)
{
    if (data.empty() || data.size() <= dof)
    {
        return -1;
    }
    return std::sqrt(variance(data, dof));
}

template <typename InputIterator>
double stddev(InputIterator first, InputIterator last, int dof = 0)
{
    if (last - first <= dof)
    {
        return -1;
    }
    return std::sqrt(variance(first, last, dof));
}

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
statistics_draft draft(const std::vector<T> &data, int dof = 0)
{
    if (data.empty() || data.size() <= dof)
    {
        return statistics_draft{-1, -1, -1, -1};
    }
    std::size_t N = data.size();
    double _sum = sum(data);
    double _mean = _sum / N;
    double addup = std::accumulate(data.cbegin(), data.cend(), 0.,
                                   [&_mean](double a, T b) { return a + (b - _mean) * (b - _mean); });
    double _var = addup / (N - dof);
    double _stddev = std::sqrt(_var);
    return statistics_draft{_sum, _mean, _var, _stddev};
}

template <typename InputIterator>
statistics_draft draft(InputIterator first, InputIterator last, int dof = 0)
{
    using T = std::remove_cv_t<std::remove_reference_t<decltype(*first)>>;
    static_assert(std::is_arithmetic_v<T>, "statistics only for arithmetic type");
    if (last - first <= dof)
    {
        return statistics_draft{-1, -1, -1, -1};
    }
    int64_t N = last - first;
    double _sum = sum(first, last);
    double _mean = _sum / N;
    double addup = std::accumulate(first, last, 0., [&_mean](double a, T b) { return a + (b - _mean) * (b - _mean); });
    double _var = addup / (N - dof);
    double _stddev = std::sqrt(_var);
    return statistics_draft{_sum, _mean, _var, _stddev};
}

} // namespace util

#endif // UTIL_STATISTICS_HPP