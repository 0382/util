#include "nucleus.hpp"
#include <iostream>

using namespace util;

int main(int argc, char const *argv[])
{
    auto iso = util::Isotope("Ne20");
    auto [z, n] = iso.valence(util::sd_space);
    std::cout << z << ',' << n << std::endl;
    std::cout << util::m_config_size<HO_size(8)>(HO_orbits(8), 2, 2);
    return 0;
}
