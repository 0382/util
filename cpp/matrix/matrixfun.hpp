/**
 * @author:      0.382
 * @description: 矩阵相关的一些算法
 */

#pragma once
#ifndef UTIL_MATRIXFUN_HPP
#define UTIL_MATRIXFUN_HPP

#include "matrix.hpp"
#include <cstdint>

namespace util
{

using mati32 = Matrix<int32_t>;
using mati64 = Matrix<int64_t>;
using matf32 = Matrix<float>;
using matf64 = Matrix<double>;

// 高斯消元法解方程
matf64 GEM(matf64 A, matf64 b);

} // namespace util

#endif // UTIL_MATRIXFUN_HPP