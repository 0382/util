#include "argparse.hpp"
#include "range.hpp"

#define VERSION "0.0.1"

namespace util{

template <>
inline std::string type_string<StepRange>()
{
    return "StepRange";
}

template <>
StepRange parse_value<StepRange>(const std::string &str)
{
    auto pos = str.find(':');
    if (pos == std::string::npos)
    {
        throw(std::invalid_argument("invalid StepRange format"));
    }
    int64_t m_begin = std::stoll(str.substr(0, pos));
    auto next_pos = str.find(':', pos + 1);
    if (next_pos == std::string::npos)
    {
        throw(std::invalid_argument("invalid StepRange format"));
    }
    int64_t m_step = std::stoll(str.substr(pos + 1, next_pos - pos - 1));
    int64_t m_end = std::stoll(str.substr(next_pos + 1));
    return range(m_begin, m_end, m_step);
}

}

int main(int argc, char const *argv[])
{
    auto args = util::argparser("A quantum physics calculation program.");
    args.set_program_name("test")
        .add_help_option()
        .add_sc_option("-v", "--version", "show version info", []() {
            std::cout << "version " << VERSION << std::endl;
        })
        .add_option("-o", "--openmp", "use openmp or not")
        .add_option("-m", "--mpi", "use mpi or not")
        .add_option<int>("-t", "--threads", "if openmp it set,\nuse how many threads,\ndefault is 4", 4)
        .add_option<util::StepRange>("-r", "--range", "range", util::range(0, 10, 2))
        .add_named_argument<std::string>("input", "initialize file")
        .add_named_argument<std::string>("output", "output file")
        .parse(argc, argv);
    if(args.has_option("--openmp"))
    {
        std::cout << "openmp is used, and we use " << args.get_option_int("--threads") << " threads" << std::endl;
    }
    if(args.has_option("--mpi"))
    {
        std::cout << "mpi is used" << std::endl;
    }
    std::cout << "calculate range is " << args.get_option<util::StepRange>("--range") << std::endl;
    std::cout << "the input file is " << args.get_argument<std::string>("input") << std::endl;
    std::cout << "the output file is " << args.get_argument<std::string>("output") << std::endl;
    args.print_as_ini(std::cout);
    return 0;
}
