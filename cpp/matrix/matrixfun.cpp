/**
 * @author:      0.382
 * @description: 矩阵相关的一些算法
 */

#include "matrixfun.hpp"
#include <cmath>

namespace util
{

// 高斯消元法
// 第一个参数必须是方阵
// 第二个参数必须是列矩阵
matf64 GEM(const matf64 &_A, const matf64 &_b)
{
    if (_b.cols() != 1)
        stop("(GEM) b should be a column vector");
    if (_A.rows() != _A.cols())
        stop("(GEM) Matrix A should be square matrix");
    if (_A.cols() != _b.rows())
        stop("(GEM) Matrix A's columns should be equals to b's rows");
    // 后面的运算要修改矩阵内部数据
    auto A = _A;
    auto b = _b;
    auto size = A.rows();
    // 化为上三角矩阵
    for (std::size_t i = 0; i < size; ++i)
    {
        // 部分支点遴选
        double max_aii = std::abs(A(i, i));
        std::size_t k = i;
        for (std::size_t r = i + 1; r < size; ++r)
        {
            if (std::abs(A(r, i)) > max_aii)
            {
                max_aii = std::abs(A(r, i));
                k = r;
            }
        }
        if (max_aii == 0)
            stop("(GEM) Cannot be simplified to upper triangular matrix!");
        // 交换第 i 行和第 k 行
        for (std::size_t j = i; j < size; ++j)
        {
            std::swap(A(i, j), A(k, j));
        }
        std::swap(b(i), b(k));
        // 将 a_ii 以下的元素变为零
        for (std::size_t s = i + 1; s < size; ++s) // 第 s 行
        {
            double rate = A(s, i) / A(i, i);
            for (std::size_t t = i; t < size; ++t) // 第 t 列
                A(s, t) -= rate * A(i, t);
            b(s) -= rate * b(i);
        }
    }
    // 反代出结果
    matf64 x(size); // 保存结果
    for (std::size_t i = size - 1; i != std::size_t(-1); --i)
    {
        x(i) = b(i);
        for (std::size_t k = size - 1; k > i; --k)
            x(i) -= A(i, k) * x(k);
        x(i) /= A(i, i);
    }
    return x;
}

// 实对称矩阵的 cholesky 分解
// 参数是一个实对称矩阵 A, 分解为 H^\dagger H
// 分解的结果是下三角矩阵 H^\dagger
matf64 cholesky(const matf64 &_A)
{
    if (_A.rows() != _A.cols())
        stop("(cholesky) Matrix A should be a square matrix");
    auto A = _A;
    A(0, 0) = std::sqrt(A(0, 0));
    for (std::size_t i = 1; i < A.rows(); ++i)
    {
        for (std::size_t j = 0; j < i; ++j)
        {
            // H(i,j) = A(i,j);
            for (std::size_t k = 0; k < j; ++k)
            {
                A(i, j) -= A(i, k) * A(j, k);
            }
            A(i, j) /= A(j, j);
        }
        // H(i,i) = A(i,i);
        for (std::size_t k = 0; k < i; ++k)
        {
            A(i, i) -= A(i, k) * A(i, k);
        }
        A(i, i) = std::sqrt(A(i, i));
    }
    // 这样在原内存上计算，必须要手动将上三角部分置为零
    for (std::size_t i = 0; i < A.rows(); ++i)
    {
        for (std::size_t j = i + 1; j < A.cols(); ++j)
        {
            A(i, j) = 0;
        }
    }
    return A;
}

// 通过 cholesky 分解求解方程 Ax = b
// 返回值为方程解
matf64 solve_by_cholesky(const matf64 &A, const matf64 &b)
{
    if (b.cols() != 1)
        stop("(solve_by_cholesky) b should be a column vector");
    if (A.rows() != A.cols())
        stop("(solve_by_cholesky) Matrix A should be square matrix");
    if (A.cols() != b.rows())
        stop("(solve_by_cholesky) Matrix A's columns should be equals to b's rows");

    auto H = cholesky(A);
    std::size_t size = A.rows();
    matf64 x(size);
    // H^\dagger * y = b
    for (std::size_t i = 0; i < size; ++i)
    {
        x(i) = b(i);
        for (std::size_t k = 0; k < i; ++k)
            x(i) -= H(i, k) * x(k);
        x(i) /= H(i, i);
    }
    // H * x = y
    for (std::size_t i = size - 1; i != std::size_t(-1); --i)
    {
        for (std::size_t k = i + 1; k < size; ++k)
            x(i) -= H(k, i) * x(k);
        x(i) /= H(i, i);
    }
    return x;
}

// 三对角矩阵的分解
// 设定所有的向量长度一致，是为了和教材上的下标一致，其实也没有太大的影响
// 返回是方程的解
matf64 Thomas(const matf64 &_A, const matf64 &_B, const matf64 &C, const matf64 &b)
{
    if (_A.cols() != 1 || _B.cols() != 1 || C.cols() != 1 || b.cols() != 1)
        stop("(Thomas) all parameters should be column vectors");
    // 分解
    auto A = _A;
    auto B = _B;
    std::size_t size = A.rows();
    for (std::size_t i = 1; i < size; ++i)
    {
        B(i) /= A(i - 1);
        A(i) -= B(i) * C(i - 1);
    }
    // 反代
    matf64 x(size);
    // Ly = b
    x(0) = b(0);
    for (std::size_t i = 1; i < size; ++i)
        x(i) = b(i) - B(i) * x(i - 1);
    // Ux = y
    x(size - 1) = x(size - 1) / A(size - 1);
    for (std::size_t i = size - 2; i != std::size_t(-1); --i)
        x(i) = (x(i) - x(i + 1) * C(i)) / A(i);
    return x;
}

} // end namespace util