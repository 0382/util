#include "statistics.hpp"
#include <iostream>

using namespace util;

int main(int argc, char const *argv[])
{
    std::vector<int> x(100, 0);
    std::iota(x.begin(), x.end(), 1);
    std::cout << "1->100:\nsum = " << sum(x) << "\n";
    std::cout << "mean = " << mean(x) << "\n";
    std::cout << "variance = " << variance(x) << "\n";
    std::cout << "sample variance = " << variance(x, 1) << "\n";

    auto dft = draft(x);
    std::cout << "sum = " << dft.sum << "\n";
    std::cout << "mean = " << dft.mean << "\n";
    std::cout << "variance = " << dft.variance << "\n";
    std::cout << "stddev = " << dft.stddev << "\n";
    return 0;
}
