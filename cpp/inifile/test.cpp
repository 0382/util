#include "inifile.hpp"

int main(){
    auto ini = inifile("test.ini");
    if(!ini.good())
    {
        std::cerr << ini.error() << std::endl;
        exit(-1);
    }
    std::cout << "default section: name = " << ini.get_string("name") << '\n';
    ini.set_string("set", "string");

    std::cout << "section1: test = "   
              << ini.section("section1").get_int("test") << '\n';

    std::cout << "\nshow all data:\n--------------\n";
    ini.show();
}