/**
 * @author:      0.382
 * @description: 矩阵相关的一些算法
 */

#pragma once
#ifndef UTIL_MATRIXFUN_HPP
#define UTIL_MATRIXFUN_HPP

#include "matrix.hpp"
#include <cstdint>
#include <valarray>

namespace util
{

using veci32 = std::valarray<int32_t>;
using veci64 = std::valarray<int64_t>;
using vecf32 = std::valarray<float>;
using vecf64 = std::valarray<double>;
using mati32 = Matrix<int32_t>;
using mati64 = Matrix<int64_t>;
using matf32 = Matrix<float>;
using matf64 = Matrix<double>;

// 高斯消元法解方程
matf64 GEM(const matf64 &A, const matf64 &b);
// chokesky分解
matf64 cholesky(const matf64 &A);
// chokesky分解解方程 Ax = b
matf64 solve_by_cholesky(const matf64 &A, const matf64 &b);

} // namespace util

#endif // UTIL_MATRIXFUN_HPP