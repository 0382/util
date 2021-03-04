/**
 * @author:      0.382
 * @description: 矩阵相关的一些算法
 */

#include "matrixfun.hpp"

namespace util
{

// 高斯消元法
matf64 GEM(matf64 A, matf64 b)
{
    if (b.cols() != 1)
        stop("(GEM) b should be a column vector");
    if (A.rows() != A.cols())
        stop("(GEM) Matrix A should be square matrix");
    if (A.cols() != b.rows())
        stop("(GEM) Matrix A's columns should be equals to b's rows");
    auto size = A.rows();
    // 化为上三角矩阵
    for (std::size_t i = 0; i < size; ++i)
    {
        // 部分支点遴选
        double max_aii = A(i, i);
        std::size_t k = i;
        for (std::size_t r = i + 1; r < size; ++r)
        {
            if (A(r, i) > max_aii)
            {
                max_aii = A(r, i);
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
    matf64 x(size, 1); // 保存结果
    for (std::size_t i = size - 1; i != std::size_t(-1); --i)
    {
        x(i) = b(i);
        for (std::size_t k = size - 1; k > i; --k)
            x(i) -= A(i, k) * x(k);
        x(i) /= A(i, i);
    }
    return x;
}

} // end namespace util