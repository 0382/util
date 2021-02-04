#include "argparse.hpp"

int main(int argc, char const *argv[])
{
    auto args = argparse::argparser("A quantum calculation program");
    args.set_program_name("test")
        .add_option("-o", "--openmp", "use openmp or not")
        .add_option("-m", "--mpi", "use mpi or not")
        .add_option("-t", "--threads", "how many threads to use", 4)
        .add_argument("input", "initialize file")
        .parse(argc, argv);
    std::cout << args.get_argument("input") << std::endl;
    std::cout << args.get_option<int>("-t") << std::endl;
    std::cout << args.get_option<bool>("--mpi") << std::endl;
    return 0;
}
