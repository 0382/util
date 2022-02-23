#include "random.hpp"
#include <iostream>

using namespace util;

int main()
{
    Random<std::mt19937> rng;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << rng.randint(0, i) << ' ' << rng.randreal(0, i) << std::endl;
    }
    return 0;
}
