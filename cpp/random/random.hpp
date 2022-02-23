#pragma once
#ifndef UTIL_RANDOM_HPP
#define UTIL_RANDOM_HPP

#include <random>

namespace util
{

// c++标准库的伪随机数太难用了，包装一下
template <typename Generator = std::default_random_engine>
class Random
{
  public:
    // 构造函数
    Random(int seed = 0) { rng.seed(seed); }
    // 设置种子
    void seed(int s) { rng.seed(s); }

    // 不提供一个参数的借口，因为这不管怎么做都有点别扭
    // 而且使用 `randint(0, n)`也并不会因此损失性能
    int randint(int a, int b) { return int_dist(rng, std::uniform_int_distribution<int>::param_type{a, b}); }
    int reanint()
    {
        return int_dist(rng, std::uniform_int_distribution<int>::param_type{0, std::numeric_limits<int>::max()});
    }

    double randreal(double a, double b)
    {
        return real_dist(rng, std::uniform_real_distribution<double>::param_type{a, b});
    }
    double randreal() { return real_dist(rng, std::uniform_real_distribution<double>::param_type{0., 1.}); }

  private:
    Generator rng;
    std::uniform_int_distribution<int> int_dist;
    std::uniform_real_distribution<double> real_dist;
};

} // end namespace util

#endif // UTIL_RANDOM_HPP