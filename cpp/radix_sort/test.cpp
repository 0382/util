#include "radix_sort.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>


using namespace util;

int main(int argc, char const *argv[])
{
    int x[] = {3, 4, 2, -1, 7, -5, -9, 6, -8, 0};
    radix_sort(10, x);
    for (int i = 0; i < 10; ++i)
    {
        std::cout << x[i] << ',';
    }
    std::cout << '\n';
    double y[] = {3.1, 4.2, 2.3, -1.4, 7.5, -5.6, -9.7, 6.8, -8.9, 0.0};
    radix_sort(10, y);
    for (int i = 0; i < 10; ++i)
    {
        std::cout << y[i] << ',';
    }
    std::cout << '\n';

    constexpr std::size_t N = 10'000'000;
    std::vector<int> x1(N);
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;
    std::for_each(x1.begin(), x1.end(), [&](int &x) { x = dist(rng); });
    std::vector<int> x2 = x1;
    auto t1 = std::chrono::system_clock::now();
    std::sort(x1.begin(), x1.end());
    auto t2 = std::chrono::system_clock::now();
    radix_sort(x2.size(), x2.data());
    auto t3 = std::chrono::system_clock::now();
    std::cout << (t2 - t1).count() << ',' << x1[N / 4] << '\n';
    std::cout << (t3 - t2).count() << ',' << x2[N / 4] << '\n';

    std::uniform_real_distribution<float> f_dist{-1, 1};
    std::vector<float> y1(N);
    std::for_each(y1.begin(), y1.end(), [&](float &x) { x = f_dist(rng); });
    std::vector<float> y2 = y1;
    t1 = std::chrono::system_clock::now();
    std::sort(y1.begin(), y1.end());
    t2 = std::chrono::system_clock::now();
    radix_sort(y2.size(), y2.data());
    t3 = std::chrono::system_clock::now();
    std::cout << (t2 - t1).count() << ',' << y1[N / 4] << '\n';
    std::cout << (t3 - t2).count() << ',' << y2[N / 4] << '\n';
    return 0;
}
