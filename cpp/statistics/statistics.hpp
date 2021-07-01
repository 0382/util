/**
 * @author: 0.382
 * @description: some functions for statistics
 * @url: https://github.com/0382/util/tree/main/cpp/statistics
 */

#pragma once
#ifndef UTIL_STATISTICS_HPP
#define UTIL_STATISTICS_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <tuple>
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
};

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
T sum(const std::vector<T> &data)
{
    return std::accumulate(data.cbegin(), data.cend(), static_cast<T>(0));
}

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
double mean(const std::vector<T> &data)
{
    if (data.empty())
    {
        return 0.;
    }
    else
    {
        return sum(data) / static_cast<double>(data.size());
    }
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

} // namespace util

#endif // UTIL_STATISTICS_HPP