#include "range.hpp"
#include <iostream>

using namespace util;

template <typename RNG>
void show_range(const char *name, RNG rng)
{
    std::cout << name << ": ";
    for (auto i : rng)
        std::cout << i << ',';
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    show_range("range(10)", range(10));
    show_range("range(23,35)", range(23, 35));
    show_range("range(45,100,3)", range(45, 100, 3));
    show_range("range(36,12,-3)", range(36, 12, -3));
    show_range("range(12,34,-4)", range(12, 34, -4));
    return 0;
}
