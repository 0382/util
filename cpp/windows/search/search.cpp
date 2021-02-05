#include "argparse.hpp"
#include "search.hpp"

int main(int argc, char const *argv[])
{
    auto args = argparse::argparser("search files in command line (only for windows)");
    args.set_program_name("search")
        .add_option<std::string>("-d", "--dir", "sepcify directory", ".")
        .add_argument("pattern", "search pattern")
        .parse(argc, argv);
    
    // 使用windows API获得完整路径
    char *dir_name = new char [MAX_PATH];
    SetCurrentDirectoryA(args.get_option<std::string>("--dir").c_str());
    GetCurrentDirectoryA(MAX_PATH, dir_name);
    std::string dir = dir_name;
    if(dir.substr(2) == "\\") dir = dir.substr(0, 2);

    std::regex search_pattern(args.get_argument("pattern"), std::regex::icase);
    search_file_with_regex(dir, search_pattern);
    return 0;
}
