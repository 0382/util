#include "nucleus.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto iso = util::Isotope("Ne20");
    auto [z, n] = iso.valence(util::sd_space);
    std::cout << z << ',' << n << std::endl;
    return 0;
}
