#include "easyprint.hpp"
#include "tryparse.hpp"

using namespace util;

int main(int argc, char const *argv[])
{
    auto a = tryparse<int>("1234", 16);
    if (a)
        print(a.value());
}
