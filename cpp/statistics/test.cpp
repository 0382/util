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

    std::cout << draft(x.cbegin(), x.cend(), 1);
    return 0;
}
