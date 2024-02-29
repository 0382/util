#include "inifile.hpp"

int main()
{
    auto ini = util::inifile("test.ini");
    if (!ini.good())
    {
        std::cerr << ini.error() << std::endl;
        std::exit(-1);
    }

    std::cout << "inifile sections:";
    for(const auto sec : ini)
    {
        std::cout << " " << sec.name();
    }
    std::cout << '\n';


    std::cout << "in default section: name = " << ini.get_string("name") << '\n';
    ini.set_string("set", "string");

    std::cout << "in section `sec1`: test = " << ini.section("sec1").get_int("test") << '\n';

    std::cout << "int section `sec2`:\n";
    for(const auto [k, v] : ini.section("sec2"))
    {
        std::cout << k << " = " << v << '\n';
    }

    std::cout << "\nshow all data:\n--------------\n";
    ini.show();

}