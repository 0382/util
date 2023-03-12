#include <cstdint>
#include <iostream>

constexpr uint64_t primemask[] = {0x816d129a64b4cb6e, 0x2196820d864a4c32, 0xa48961205a0434c9, 0x4a2882d129861144,
                                  0x834992132424030,  0x148a48844225064b, 0xb40b4086c304205,  0x65048928125108a0};

// only useful for n <= 1024
bool small_isprime(int n)
{
    if (n == 2)
        return true;
    if (n < 0 || n >= 1024 || !(n & 1))
        return false;
    n = n >> 1;
    return primemask[n >> 6] & (uint64_t(1) << (n & 63));
}

int main(int argc, char const *argv[])
{
    int count = 0;
    for (int i = 0; i < 1024; ++i)
    {
        if (small_isprime(i))
        {
            ++count;
            std::cout << i << ' ';
        }
    }
    std::cout << "\ncount = " << count << '\n';
}