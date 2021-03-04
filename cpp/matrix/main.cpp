#include "matrixfun.hpp"

using namespace util;

// 高斯消元法解方程测试
void test_GEM()
{
    matf32 A(2, 2); // 写了矩阵之间的类型转换
    A.set_value({1, 2, 3, 4});
    matf64 b(2, 1);
    b.set_value({5, 6});
    matf64 x = GEM(A, b); // matf32 隐式转换为 matf64
    print(A);
    print(b);
    print(x);
}

int main(int argc, char const *argv[])
{
    test_GEM();

    matf32 A(2, 2);
    A.set_value({1, 2, 3, 4});
    mati64 B(2, 2);
    B.set_value({1, 2, 3, 4});
    // print(A + B); // 但是这样会出错，因为不知道是 i64 转 f32，还是反过来
    print(A + static_cast<matf32>(B)); // 如果你想的话，这样也不是不可以
    return 0;
}
