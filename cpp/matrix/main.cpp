#include "matrixfun.hpp"
#include <random>

using namespace util;

// 测试隐私类型转换
void test_cast()
{
    matf32 A(2, 2); // 写了矩阵之间的类型转换
    A.set_value({1, 2, 3, 4});
    matf64 b(2, 1);
    b.set_value({5, 6});
    matf64 x = GEM(A, b); // matf32 隐式转换为 matf64
    mati64 B(2, 2);
    B.set_value({1, 2, 3, 4});
    // print(A + B); // 但是这样会出错，因为不知道是 i64 转 f32，还是反过来
    print(A + static_cast<matf32>(B)); // 如果你想的话，这样也不是不可以
}

// 高斯消元法解方程测试
void test_GEM(std::size_t size)
{
    matf64 A(size, size);
    matf64 b(size, 1);
    for (int i = 0; i < size; ++i)
    {
        b(i) = std::rand() / double(RAND_MAX);
        for (int j = 0; j < size; ++j)
        {
            A(i, j) = std::rand() / double(RAND_MAX);
        }
    }
    auto x = GEM(A, b);
    if (approx(matmul(A, x), b, 1e-10))
    {
        println("test GEM passed");
    }
    else
    {
        println("test GEM failed");
    }
}

// cholesky分解解方程测试
void test_cholesky(std::size_t size)
{
    matf64 A(size, size);
    matf64 b(size, 1);
    for (int i = 0; i < size; ++i)
    {
        b(i) = std::rand() / double(RAND_MAX);
        for (int j = i; j < size; ++j)
        {
            A(i, j) = std::rand() / double(RAND_MAX);
            A(j, i) = A(i, j);
        }
    }
    auto x = solve_by_cholesky(A, b);
    if (approx(matmul(A, x), b, 1e-10))
    {
        println("test cholesky passed");
    }
    else
    {
        println("test cholesky failed");
    }
}

int main(int argc, char const *argv[])
{
    // test_cast();
    for (int i = 1; i <= 100; ++i)
    {
        test_GEM(i);
        test_cholesky(i);
    }
    return 0;
}
