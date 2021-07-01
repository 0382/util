/**
 * @author: 0.382
 * @description: parse command line args
 * @url: https://github.com/0382/util/tree/main/cpp/argparse
 */

#pragma once
#ifndef JSHL_ARGPARSE_HPP
#define JSHL_ARGPARSE_HPP

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace util
{

template <typename T>
struct type_string
{
    static constexpr const char *value = "null";
};

template <>
struct type_string<bool>
{
    static constexpr const char *value = "bool";
};

template <>
struct type_string<int>
{
    static constexpr const char *value = "int";
};

template <>
struct type_string<double>
{
    static constexpr const char *value = "double";
};

template <>
struct type_string<std::string>
{
    static constexpr const char *value = "string";
};

template <>
struct type_string<const char *>
{
    static constexpr const char *value = "string";
};

template <typename T>
T value_cast(const std::string &s)
{
    std::istringstream iss(s);
    T value;
    iss >> value;
    return value;
}

template <>
bool value_cast<bool>(const std::string &s)
{
    return s != "";
}

template <>
int value_cast<int>(const std::string &s)
{
    return std::stoi(s);
}

template <>
double value_cast<double>(const std::string &s)
{
    return std::stod(s);
}

template <>
std::string value_cast<std::string>(const std::string &s)
{
    return s;
}

struct option
{
    option(std::string _short, std::string _long, std::string _help, std::string _type, std::string _value)
        : short_str(std::move(_short)), long_str(std::move(_long)), help(std::move(_help)), type(std::move(_type)),
          value(std::move(_value))
    {}

    void print_help() const
    {
        if (this->short_str == "")
            std::cout << "     ";
        else
            std::cout << std::setw(4) << this->short_str << ",";
        if (this->long_str == "")
            std::cout << "          ";
        else
            std::cout << std::setw(10) << this->long_str;
        std::cout << "          " << this->help;
        if (this->type != "bool")
        {
            std::cout << " (" << this->type << ")";
        }
        std::cout << std::endl;
    }

    std::string short_str;
    std::string long_str;
    std::string help;
    std::string type;
    std::string value;
};

struct argument
{
    argument(std::string _name, std::string _help) : name(std::move(_name)), help(std::move(_help)) {}

    bool has() const { return this->value != ""; }
    void print_help() const { std::cout << std::setw(15) << this->name << "          " << this->help << std::endl; }

    std::string name;
    std::string help;
    std::string value;
};

class argparser
{
  public:
    argparser(std::string _description) : description(std::move(_description))
    {
        this->add_option("-h", "--help", "show this help message");
    }

    argparser &set_program_name(std::string _program_name)
    {
        this->program_name = std::move(_program_name);
        return *this;
    }

    argparser &add_argument(const std::string &arg, const std::string &help)
    {
        auto pos = this->find_argument(arg);
        if (pos == arguments.cend())
        {
            this->arguments.push_back(argument(arg, help));
        }
        else
        {
            std::cerr << "argument (" << pos->name << ") has been added" << std::endl;
            std::exit(-1);
        }
        return *this;
    }

    template <typename T>
    argparser &add_option(const std::string &_short, const std::string &_long, const std::string &_help, T &&_value)
    {
        auto pos_short = this->find_option_short(_short);
        auto pos_long = this->find_option_long(_long);
        if (pos_short != options.cend())
        {
            std::cerr << "option (" << pos_short->short_str << ") has been added" << std::endl;
            std::exit(-1);
        }
        if (pos_long != options.cend())
        {
            std::cerr << "option (" << pos_long->long_str << ") has been added" << std::endl;
            std::exit(-1);
        }
        std::string value_str = "";
        if constexpr (strcmp("string", type_string<T>::value) == 0)
            value_str = _value;
        else
            value_str = std::to_string(_value);

        this->options.push_back(option(_short, _long, _help, type_string<T>::value, value_str));
        return *this;
    }

    argparser &add_option(const std::string &_short, const std::string &_long, const std::string &_help)
    {
        auto pos_short = this->find_option_short(_short);
        auto pos_long = this->find_option_long(_long);
        if (pos_short != options.cend())
        {
            std::cerr << "option (" << pos_short->short_str << ") has been added" << std::endl;
            std::exit(-1);
        }
        if (pos_long != options.cend())
        {
            std::cerr << "option (" << pos_long->long_str << ") has been added" << std::endl;
            std::exit(-1);
        }
        this->options.push_back(option(_short, _long, _help, "bool", ""));
        return *this;
    }

    argparser &parse(int argc, char const *argv[])
    {
        if (this->program_name == "")
        {
            this->program_name = argv[0];
        }
        if (argc == 1)
        {
            this->print_usage();
            std::exit(0);
        }
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }
        for (auto &opt : this->options)
        {
            auto pos = std::find_if(args.cbegin(), args.cend(),
                                    [&opt](const std::string &s) { return s == opt.long_str || s == opt.short_str; });
            if (pos == args.cend())
                continue;
            pos = args.erase(pos);
            if (opt.type == "bool")
            {
                opt.value = "true";
                continue;
            }
            if (pos == args.cend())
            {
                std::cerr << "option (" << opt.long_str << ") should have value" << std::endl;
                std::exit(-1);
            }
            else
            {
                opt.value = *pos;
                pos = args.erase(pos);
            }
        }
        this->check_help();
        if (this->arguments.size() != args.size())
        {
            std::cerr << "argument number missmatching, give " << args.size() << ", but need " << this->arguments.size()
                      << std::endl;
            std::exit(-1);
        }
        else
        {
            auto pos = args.cbegin();
            std::for_each(arguments.begin(), arguments.end(), [&pos](argument &arg) { arg.value = *pos++; });
        }
        return *this;
    }

    template <typename T>
    T get_option(const std::string &opt) const
    {
        auto pos = this->find_option(opt);
        if (pos == options.cend())
        {
            std::cerr << "invalid option (" << opt << ")\n";
            std::exit(-1);
        }
        else if (pos->type != type_string<T>::value)
        {
            std::cerr << "get type missmatching of option (" << opt << ")\n";
            exit(-1);
        }
        return value_cast<T>(pos->value);
    }

    std::string get_argument(const std::string &arg) const
    {
        auto pos = this->find_argument(arg);
        if (pos == this->arguments.end())
        {
            std::cerr << "invalid argument (" << arg << ")\n";
            std::exit(-1);
        }
        return pos->value;
    }

  private:
    std::string description;
    std::string error;
    std::string program_name;
    std::vector<option> options;
    std::vector<argument> arguments;

    using argument_iterator = std::vector<argument>::const_iterator;
    using option_iterator = std::vector<option>::const_iterator;

    auto find_argument(const std::string &key) const -> argument_iterator
    {
        return std::find_if(this->arguments.cbegin(), this->arguments.cend(),
                            [&key](const argument &arg) { return arg.name == key; });
    }
    auto find_option_short(const std::string &key) const -> option_iterator
    {
        return std::find_if(this->options.cbegin(), this->options.cend(),
                            [&key](const option &opt) { return opt.short_str == key; });
    }
    auto find_option_long(const std::string &key) const -> option_iterator
    {
        return std::find_if(this->options.cbegin(), this->options.cend(),
                            [&key](const option &opt) { return opt.long_str == key; });
    }
    auto find_option(const std::string &key) const -> option_iterator
    {
        return std::find_if(this->options.cbegin(), this->options.cend(),
                            [&key](const option &opt) { return opt.long_str == key || opt.short_str == key; });
    }

    void print_usage() const
    {
        std::cout << "usage: " << this->program_name << " [options]";
        std::for_each(this->arguments.cbegin(), this->arguments.cend(),
                      [](const argument &arg) { std::cout << " " << arg.name; });
        std::cout << std::endl;
    }

    void print_help() const
    {
        std::cout << this->description << "\n";
        this->print_usage();
        if (!this->arguments.empty())
        {
            std::cout << "arguments:\n";
            std::for_each(this->arguments.cbegin(), this->arguments.cend(),
                          [](const argument &arg) { arg.print_help(); });
        }
        std::cout << "options:\n";
        std::for_each(this->options.cbegin(), this->options.cend(), [](const option &opt) { opt.print_help(); });
    }

    void check_help() const
    {
        if (this->get_option<bool>("--help"))
        {
            this->print_help();
            std::exit(0);
        }
    }
};

} // end namespace util

#endif // JSHL_ARGPARSE_HPP
