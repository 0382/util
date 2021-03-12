#include "easyprint.hpp"
#include "range.hpp"

using namespace util;

int main(int argc, char const *argv[])
{
    print("range(10): ");
    for (auto i : range(10))
        print(i, ',');
    print("\nrange(23,35): ");
    for (auto i : range(23, 35))
        print(i, ',');
    print("\nrange(45,100,3): ");
    for (auto i : range(45, 100, 3))
        print(i, ',');
    print("\nrange(36,12,-3): ");
    for (auto i : range(36, 12, -3))
        print(i, ',');
    print("\nrange(12,34,-4): ");
    for (auto i : range(12, 34, -4))
        print(i, ',');
    print("\n");
    return 0;
}
