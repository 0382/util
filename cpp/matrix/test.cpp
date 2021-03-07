#include "integral.hpp"
#include "matrixfun.hpp"
#include <random>

using namespace util;

// 测试隐式类型转换
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
    if (approx(matmul(A, x), b))
        println("test GEM passed");
    else
        println("test GEM failed");
}

// cholesky分解解方程测试
void test_cholesky(std::size_t size)
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
    // 随机矩阵并不一定是正定的
    // 这样构造只要 A 不是奇异的，就是正定的
    A = matmul(A, transpose(A));
    auto x = solve_by_cholesky(A, b);
    // println("x = \n", x);
    if (approx(matmul(A, x), b))
        println("test cholesky passed");
    else
        println("test cholesky failed");
}

void test_Thomas(std::size_t size)
{
    matf64 M(size, size);
    matf64 A(size), B(size), C(size), b(size);
    for (int i = 0; i < size; ++i)
    {
        A(i) = std::rand() / double(RAND_MAX);
        B(i) = std::rand() / double(RAND_MAX);
        C(i) = std::rand() / double(RAND_MAX);
        b(i) = std::rand() / double(RAND_MAX);
        M(i, i) = A(i);
        if (i != 0)
            M(i, i - 1) = B(i);
        if (i != size - 1)
            M(i, i + 1) = C(i);
    }
    auto x = Thomas(A, B, C, b);
    if (approx(matmul(M, x), b))
        println("test Thomas passed");
    else
        println("test Thomas failed");
}

// 测试积分
void test_integral()
{
    auto f = [](double x) { return std::sqrt(4 - x * x); };
    std::cout << std::setprecision(18);
    std::cout << integral<double>(0, 2, f) << std::endl;
    std::cout << integral<double>(0, 2, f, {IntLevel::lv2}) << std::endl;
    std::cout << integral<double>(0, 2, f, {IntLevel::lv3}) << std::endl;
    std::cout << integral<double>(0, 2, f, {IntLevel::lv4}) << std::endl;
    std::cout << integral<double>(0, 2, f, {IntLevel::lv5}) << std::endl;
    std::cout << integral<double>(0, 2, f, {IntLevel::lv6}) << std::endl;
}

int main(int argc, char const *argv[])
{
    test_cast();
    for (int i = 1; i <= 10; ++i)
    {
        test_GEM(i);
        test_cholesky(i);
        test_Thomas(i);
    }
    test_integral();
    return 0;
}
