#include "argparse.hpp"
#include "range.hpp"

#define VERSION "0.0.1"

namespace util
{

template <>
inline constexpr std::string_view type_name<StepRange>()
{
    return "StepRange";
}

template <>
std::optional<StepRange> try_parse<StepRange>(const std::string &str)
{
    auto pos = str.find(':');
    if (pos == std::string::npos)
    {
        return std::nullopt;
    }
    int64_t m_begin = std::stoll(str.substr(0, pos));
    auto next_pos = str.find(':', pos + 1);
    if (next_pos == std::string::npos)
    {
        return std::nullopt;
    }
    int64_t m_step = std::stoll(str.substr(pos + 1, next_pos - pos - 1));
    int64_t m_end = std::stoll(str.substr(next_pos + 1));
    return range(m_begin, m_end, m_step);
}

} // namespace util

int main(int argc, char const *argv[])
{
    auto args = util::argparser("A quantum physics calculation program.");
    args.set_program_name("qcalc")
        .add_help_option()
        .use_color_error()
        .add_sc_option("-v", "--version", "show version info",
                       []() { std::cout << "qcalc version " << VERSION << std::endl; })
        .add_option("-o", "--openmp", "use openmp or not")
        .add_option("-m", "--mpi", "use mpi or not")
        .add_option<int>("-t", "--threads", "if openmp is set,\nuse how many threads,\ndefault is 16", 16)
        .add_option<int>("-p", "--processes", "if mpi is set,\nuse how many processes,\ndefault is 4", 4)
        .add_option<std::string>("", "--chemical", "chamical formula", "H2O")
        .add_option<util::StepRange>("-r", "--range", "range", util::range(0, 10, 2))
        .add_named_argument<std::string>("input", "initialize file")
        .add_named_argument<std::string>("output", "output file")
        .parse(argc, argv);
    if (args.has_option("--openmp"))
    {
        std::cout << "openmp is used, and we use " << args.get_option_int("--threads") << " threads" << std::endl;
    }
    if (args.has_option("--mpi"))
    {
        std::cout << "mpi is used, and we use " << args.get_option_int("--processes") << " processes" << std::endl;
    }
    std::cout << "the chemical formula is " << args.get_option_string("--chemical") << std::endl;
    std::cout << "calculate range is ";
    for (auto i : args.get_option<util::StepRange>("--range"))
    {
        std::cout << i << ',';
    }
    std::cout << std::endl;
    std::cout << "the input file is " << args.get_argument<std::string>("input") << std::endl;
    std::cout << "the output file is " << args.get_argument<std::string>("output") << std::endl;
    return 0;
}
