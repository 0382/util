#pragma once
#ifndef JSHL_ARGPARSE_HPP
#define JSHL_ARGPARSE_HPP

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace util
{

// 尽管使用编译器相关的 ABI 函数可以相对比较优雅的实现这个功能
// 但是不同的编译器在某些类型下可能出现一些奇怪的行为
// 最重要的是 std::string 还是免不了要模板特例化
// 因此，不如在这里限定一些类型，免去不可控制的行为

// 我们仅支持 bool, int, int64_t, double, std::string
// 想要其他长度的类型，获取值之后自行转换
// 当然，如果你愿意的话，自己定义模板特例化也不是不可以

template <typename T>
inline constexpr std::string_view type_name()
{
    return "null";
}

template <>
inline constexpr std::string_view type_name<bool>()
{
    return "bool";
}

template <>
inline constexpr std::string_view type_name<int>()
{
    return "int";
}

template <>
inline constexpr std::string_view type_name<int64_t>()
{
    return "int64_t";
}

template <>
inline constexpr std::string_view type_name<double>()
{
    return "double";
}

template <>
inline constexpr std::string_view type_name<std::string>()
{
    return "string";
}

template <typename T>
std::string to_string(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename T>
inline std::optional<T> try_parse(const std::string &str)
{
    T value;
    std::istringstream iss(str);
    iss >> value;
    if (iss.fail() || !iss.eof())
    {
        return std::nullopt;
    }
    return value;
}

template <>
inline std::optional<bool> try_parse<bool>(const std::string &str)
{
    if (str == "true" || str == "1")
    {
        return true;
    }
    if (str == "false" || str == "0")
    {
        return false;
    }
    return std::nullopt;
}

template <>
inline std::optional<std::string> try_parse<std::string>(const std::string &str)
{
    return str;
}

struct short_circuit_option
{
    short_circuit_option(std::string sname, std::string lname, std::string help, std::function<void(void)> callback)
        : short_name(std::move(sname)), long_name(std::move(lname)), help(std::move(help)),
          callback(std::move(callback))
    {}
    std::string short_name;
    std::string long_name;
    std::string help;
    std::function<void(void)> callback;
};

struct option_base
{
    option_base(std::string sname, std::string lname, std::string help)
        : short_name(std::move(sname)), long_name(std::move(lname)), help(std::move(help))
    {}
    std::string short_name;
    std::string long_name;
    std::string help;

    virtual std::string_view type() const = 0;
    virtual bool try_parse(const std::string &str) = 0;
};

template <typename T>
struct option : option_base
{
    option(std::string sname, std::string lname, std::string help, T value)
        : option_base(std::move(sname), std::move(lname), std::move(help)), value(std::move(value))
    {}

    T value;

    virtual std::string_view type() const override { return type_name<T>(); }
    virtual bool try_parse(const std::string &str) override
    {
        std::optional<T> ret = util::try_parse<T>(str);
        if (!ret)
        {
            return false;
        }
        this->value = std::move(*ret);
        return true;
    }
};

struct argument_base
{
    argument_base(std::string name, std::string help) : name(std::move(name)), help(std::move(help)) {}

    std::string name;
    std::string help;
    virtual std::string_view type() const = 0;
    virtual bool try_parse(const std::string &str) = 0;
};

template <typename T>
struct argument : argument_base
{
    argument(std::string name, std::string help) : argument_base(std::move(name), std::move(help)) {}

    std::unique_ptr<T> value;

    virtual std::string_view type() const override { return type_name<T>(); }
    virtual bool try_parse(const std::string &str) override
    {
        std::optional<T> ret = util::try_parse<T>(str);
        if (!ret)
        {
            return false;
        }
        this->value = std::make_unique<T>(std::move(*ret));
        return true;
    }
};

class argparser
{
  public:
    using error_handler = std::function<void(const std::string &err_msg)>;

  private:
    std::string description;
    std::string program_name;
    std::vector<short_circuit_option> short_circuit_options;
    std::vector<std::unique_ptr<option_base>> options;
    std::unordered_map<char, std::size_t> short_name_index;
    std::vector<std::unique_ptr<argument_base>> named_arguments;
    std::vector<std::unique_ptr<argument_base>> arguments;

    error_handler build_error;
    error_handler parse_error;
    error_handler get_error;

    static void simple_handler(const std::string &prefix, const std::string &msg)
    {
        std::cerr << prefix << msg << std::endl;
        std::exit(-1);
    }

  public:
    argparser(std::string description) : description(std::move(description)) {}

    argparser &set_program_name(std::string name)
    {
        program_name = std::move(name);
        build_error = [this](const std::string &msg) { simple_handler("build error: ", msg); };
        parse_error = [this](const std::string &msg) { simple_handler("parse error: ", msg); };
        get_error = [this](const std::string &msg) { simple_handler("get error: ", msg); };
        return *this;
    }

    argparser &use_color_error()
    {
        build_error = [this](const std::string &msg) { simple_handler("\033[31mbuild error: \033[0m", msg); };
        parse_error = [this](const std::string &msg) { simple_handler("\033[31mparse error: \033[0m", msg); };
        get_error = [this](const std::string &msg) { simple_handler("\033[31mget error: \033[0m", msg); };
        return *this;
    }

    argparser &use_exception_error()
    {
        build_error = [this](const std::string &msg) { throw std::runtime_error("build error: " + msg); };
        parse_error = [this](const std::string &msg) { throw std::runtime_error("parse error: " + msg); };
        get_error = [this](const std::string &msg) { throw std::runtime_error("get error: " + msg); };
        return *this;
    }

    argparser &reg_build_error(error_handler handler)
    {
        build_error = std::move(handler);
        return *this;
    }

    argparser &reg_parse_error(error_handler handler)
    {
        parse_error = std::move(handler);
        return *this;
    }

    argparser &reg_get_error(error_handler handler)
    {
        get_error = std::move(handler);
        return *this;
    }

    void print_usage() const
    {
        std::cout << "usage: " << program_name << " [options]";
        for (const auto &named_arg : named_arguments)
        {
            std::cout << " [=" << named_arg->name << "]";
        }
        for (const auto &arg : arguments)
        {
            std::cout << " [" << arg->name << "]";
        }
        std::cout << std::endl;
    }

    void print_help() const
    {
        print_usage();
        std::cout << "\n" << description << "\n\n";
        std::cout << "Options:\n";
        // calculate the longest option name
        std::size_t max_length = 0;
        for (const auto &opt : short_circuit_options)
        {
            std::size_t length = opt.long_name.length() + opt.short_name.empty() ? 0 : 4;
            max_length = std::max(max_length, length);
        }
        for (const auto &opt : options)
        {
            std::size_t length = opt->long_name.length() + opt->short_name.empty() ? 0 : 4;
            max_length = std::max(max_length, length);
        }
        max_length = std::max(max_length, std::size_t(25));
        // print the options
        for (const auto &opt : short_circuit_options)
        {
            std::cout << "  ";
            std::size_t printed_length = 0;
            if (!opt.short_name.empty())
            {
                std::cout << opt.short_name << ", ";
                printed_length = 4;
            }
            std::cout << opt.long_name;
            printed_length += opt.long_name.length();
            std::cout << std::string(max_length - printed_length, ' ');
            std::cout << replace(opt.help, "\n", "\n" + std::string(max_length + 2, ' ')) << '\n';
        }
        for (const auto &opt : options)
        {
            std::cout << "  ";
            std::size_t printed_length = 0;
            if (!opt->short_name.empty())
            {
                std::cout << opt->short_name << ", ";
                printed_length = 4;
            }
            std::cout << opt->long_name;
            printed_length += opt->long_name.length();
            std::cout << std::string(max_length - printed_length, ' ');
            if (opt->type() != "bool")
            {
                std::cout << "(" << opt->type() << ") ";
            }
            std::cout << replace(opt->help, "\n", "\n" + std::string(max_length + 2, ' ')) << '\n';
        }
        if (!named_arguments.empty())
        {
            std::cout << "\nNamed arguments:\n";
            max_length = 0;
            for (const auto &arg : named_arguments)
            {
                max_length = std::max(max_length, arg->name.length());
            }
            max_length = std::max(max_length, std::size_t(25));
            for (const auto &arg : named_arguments)
            {
                std::cout << "  ";
                std::cout << arg->name;
                std::cout << std::string(max_length - arg->name.length(), ' ') << "(" << arg->type() << ") ";
                std::cout << replace(arg->help, "\n", "\n" + std::string(max_length + 2, ' ')) << '\n';
            }
        }
        if (!arguments.empty())
        {
            std::cout << "\nPosition arguments:\n";
            max_length = 0;
            for (const auto &arg : arguments)
            {
                max_length = std::max(max_length, arg->name.length());
            }
            max_length = std::max(max_length, std::size_t(25));
            for (const auto &arg : arguments)
            {
                std::cout << "  ";
                std::cout << arg->name;
                std::cout << std::string(max_length - arg->name.length(), ' ') << "(" << arg->type() << ") ";
                std::cout << replace(arg->help, "\n", "\n" + std::string(max_length + 2, ' ')) << '\n';
            }
        }
    }

    argparser &add_help_option()
    {
        return add_sc_option("-?", "--help", "show this help message", [this]() { print_help(); });
    }

    // add short circuit option
    argparser &add_sc_option(std::string sname, std::string lname, std::string help, std::function<void(void)> callback)
    {
        // long name must not be empty
        check_add_option_lname(lname);
        // allow short name to be empty
        if (!sname.empty())
        {
            check_add_option_sname(sname);
            short_name_index[sname.back()] = short_circuit_options.size();
        }
        short_circuit_options.emplace_back(std::move(sname), std::move(lname), std::move(help), std::move(callback));
        return *this;
    }

    template <typename T>
    argparser &add_option(std::string sname, std::string lname, std::string help, T default_value)
    {
        if (type_name<T>() == "null")
        {
            build_error("unsupport type for option: " + std::string(typeid(T).name()));
        }
        check_add_option_lname(lname);
        if (!sname.empty())
        {
            check_add_option_sname(sname);
            short_name_index[sname.back()] = options.size();
        }
        options.emplace_back(
            std::make_unique<option<T>>(std::move(sname), std::move(lname), std::move(help), std::move(default_value)));
        return *this;
    }

    argparser &add_option(std::string sname, std::string lname, std::string help)
    {
        add_option<bool>(std::move(sname), std::move(lname), std::move(help), false);
        return *this;
    }

    template <typename T>
    argparser &add_argument(std::string name, std::string help)
    {
        check_add_argument_name<T>(name);
        arguments.emplace_back(std::make_unique<argument<T>>(std::move(name), std::move(help)));
        return *this;
    }

    template <typename T>
    argparser &add_named_argument(std::string name, std::string help)
    {
        check_add_argument_name<T>(name);
        named_arguments.emplace_back(std::make_unique<argument<T>>(std::move(name), std::move(help)));
        return *this;
    }

    template <typename T>
    T get_option(const std::string &name) const
    {
        if (name.empty())
        {
            get_error("option name cannot be empty");
        }
        auto pos = find_option_sname(name);
        if (pos == options.cend())
        {
            pos = find_option_lname(name);
        }
        if (pos == options.cend())
        {
            get_error("option not found: " + name);
        }
        auto ptr = dynamic_cast<option<T> *>(pos->get());
        if (ptr == nullptr)
        {
            std::string err_msg = "option type mismatch: set ";
            err_msg += pos->get()->type();
            err_msg += " but you try get with ";
            err_msg += type_name<T>();
            get_error(err_msg);
        }
        return ptr->value;
    }

    // some alias for get_option
    bool has_option(const std::string &name) const { return get_option<bool>(name); }
    bool get_option_bool(const std::string &name) const { return get_option<bool>(name); }
    int get_option_int(const std::string &name) const { return get_option<int>(name); }
    int64_t get_option_int64(const std::string &name) const { return get_option<int64_t>(name); }
    double get_option_double(const std::string &name) const { return get_option<double>(name); }
    std::string get_option_string(const std::string &name) const { return get_option<std::string>(name); }

    template <typename T>
    T get_argument(const std::string &name) const
    {
        auto pos = find_argument(name);
        if (pos == arguments.cend())
        {
            pos = find_named_argument(name);
        }
        if (pos == named_arguments.cend())
        {
            get_error("argument not found: " + name);
        }
        auto ptr = dynamic_cast<argument<T> *>(pos->get());
        if (ptr == nullptr)
        {
            std::string err_msg = "argument type mismatch: set ";
            err_msg += pos->get()->type();
            err_msg += " but you try get with ";
            err_msg += type_name<T>();
            get_error(err_msg);
        }
        return *ptr->value;
    }

    // some alias for get_argument
    int get_argument_int(const std::string &name) const { return get_argument<int>(name); }
    int64_t get_argument_int64(const std::string &name) const { return get_argument<int64_t>(name); }
    double get_argument_double(const std::string &name) const { return get_argument<double>(name); }
    std::string get_argument_string(const std::string &name) const { return get_argument<std::string>(name); }

    // parse arguments
    argparser &parse(int argc, char const *argv[])
    {
        // if not set program name, use argv[0]
        if (program_name.empty())
        {
            program_name = argv[0];
        }
        if (argc == 1 && (!arguments.empty() || !named_arguments.empty()))
        {
            print_usage();
            std::exit(0);
        }
        std::vector<std::string> tokens;
        for (int i = 1; i < argc; ++i)
        {
            tokens.emplace_back(argv[i]);
        }
        // start parse short circuit options
        for (auto &&sc_opt : short_circuit_options)
        {
            auto pos = std::find_if(tokens.cbegin(), tokens.cend(),
                                    [&sc_opt](const std::string &tok)
                                    { return tok == sc_opt.short_name || tok == sc_opt.long_name; });
            if (pos != tokens.cend())
            {
                sc_opt.callback();
                std::exit(0);
            }
        }
        // start parse options
        for (auto &&opt : options)
        {
            auto pos = std::find_if(tokens.cbegin(), tokens.cend(),
                                    [&opt](const std::string &tok)
                                    { return tok == opt->short_name || tok == opt->long_name; });
            if (pos == tokens.cend())
            {
                continue;
            }
            pos = tokens.erase(pos);
            // bool type need not parse next token
            if (auto ptr = dynamic_cast<option<bool> *>(opt.get()); ptr != nullptr)
            {
                ptr->value = true;
            }
            else // other types need to parse next token
            {
                std::string sep = opt->short_name.empty() ? "" : ", ";
                if (pos == tokens.cend())
                {
                    parse_error("option '" + opt->short_name + sep + opt->long_name + "' should have value");
                }
                if (!opt->try_parse(*pos))
                {
                    parse_error("option '" + opt->short_name + sep + opt->long_name +
                                "' failed to parse value: " + *pos);
                }
                pos = tokens.erase(pos);
            }
        }
        // if there are short name like options, parse it as aggregation short name options
        {
            auto pos =
                std::find_if(tokens.cbegin(), tokens.cend(), [](const std::string &tok) { return tok.front() == '-'; });
            while (pos != tokens.cend())
            {
                if (pos->length() == 1)
                {
                    parse_error("unexpected single `-` in command line arguments");
                }
                if ((*pos)[1] == '-')
                {
                    parse_error("unrecognized option '" + (*pos) + "'");
                }
                std::string short_names = pos->substr(1);
                for (char ch : short_names)
                {
                    std::size_t index = short_name_index[ch];
                    if (index < short_circuit_options.size() && short_circuit_options[index].short_name.back() == ch)
                    {
                        short_circuit_options[index].callback();
                        std::exit(0);
                    }
                }
                for (char ch : short_names)
                {
                    std::size_t index = short_name_index[ch];
                    if (index < options.size() && options[index]->short_name.back() == ch)
                    {
                        auto ptr = dynamic_cast<option<bool> *>(options[index].get());
                        if (ptr == nullptr)
                        {
                            parse_error("aggregation short name option must be bool");
                        }
                        ptr->value = true;
                    }
                    else
                    {
                        parse_error("unrecognized short name option '" + std::string(1, ch) + "' in " + (*pos));
                    }
                }
                pos = tokens.erase(pos);
                pos = std::find_if(pos, tokens.cend(), [](const std::string &tok) { return tok.front() == '-'; });
            }
        }
        // start parse named arguments
        if (tokens.size() < named_arguments.size())
        {
            parse_error("not enough named_arguments");
        }
        for (auto &named_arg : named_arguments)
        {
            for (auto pos = tokens.begin(); pos != tokens.end();)
            {
                std::string arg_value;
                if (try_parse_kv(*pos, named_arg->name, arg_value))
                {
                    if (!named_arg->try_parse(arg_value))
                    {
                        parse_error("named_argument " + named_arg->name + " failed to parse value: " + arg_value);
                    }
                    pos = tokens.erase(pos);
                    break;
                }
                ++pos;
            }
        }
        // start parse position arguments
        if (tokens.size() != arguments.size())
        {
            std::string err_msg = "position argument number missmatching, give " + std::to_string(tokens.size()) +
                                  ", but need " + std::to_string(arguments.size());
            err_msg += "\nuncaptured command line arguments: ";
            bool first = true;
            for (const auto &tok : tokens)
            {
                err_msg += (first ? "'" : ", '") + tok + "'";
                first = false;
            }
            parse_error(err_msg);
        }
        for (std::size_t i = 0; i < tokens.size(); ++i)
        {
            if (!arguments[i]->try_parse(tokens[i]))
            {
                parse_error("position argument " + arguments[i]->name + " failed to parse value: " + tokens[i]);
            }
        }
        return *this;
    }

  private:
    bool try_parse_kv(const std::string &line, const std::string &arg_name, std::string &arg_value) const
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
        {
            return false;
        }
        auto name = line.substr(0, pos);
        auto value = line.substr(pos + 1);
        if (name != arg_name)
        {
            return false;
        }
        arg_value = value;
        return true;
    }

    std::string replace(const std::string &str, const std::string &from, const std::string &to) const
    {
        std::string result = str;
        std::size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos)
        {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        return result;
    }

    using argument_iterator = std::vector<std::unique_ptr<argument_base>>::const_iterator;
    using option_iterator = std::vector<std::unique_ptr<option_base>>::const_iterator;
    using sc_option_iterator = std::vector<short_circuit_option>::const_iterator;

    auto find_argument(const std::string &key) const -> argument_iterator
    {
        return std::find_if(arguments.cbegin(), arguments.cend(), [&key](const auto &arg) { return arg->name == key; });
    }

    auto find_named_argument(const std::string &key) const -> argument_iterator
    {
        return std::find_if(named_arguments.cbegin(), named_arguments.cend(),
                            [&key](const auto &arg) { return arg->name == key; });
    }

    auto find_sc_option_sname(const std::string &key) const -> sc_option_iterator
    {
        return std::find_if(short_circuit_options.cbegin(), short_circuit_options.cend(),
                            [&key](const short_circuit_option &opt) { return opt.short_name == key; });
    }

    auto find_sc_option_lname(const std::string &key) const -> sc_option_iterator
    {
        return std::find_if(short_circuit_options.cbegin(), short_circuit_options.cend(),
                            [&key](const short_circuit_option &opt) { return opt.long_name == key; });
    }

    auto find_option_sname(const std::string &key) const -> option_iterator
    {
        return std::find_if(options.cbegin(), options.cend(),
                            [&key](const auto &opt) { return opt->short_name == key; });
    }

    auto find_option_lname(const std::string &key) const -> option_iterator
    {
        return std::find_if(options.cbegin(), options.cend(),
                            [&key](const auto &opt) { return opt->long_name == key; });
    }

    void check_add_option_sname(const std::string &key) const
    {
        if (key.size() != 2 || key.front() != '-')
        {
            build_error("short option name must be `-` followed by one character");
        }
        char ch = key.back();
        if (short_name_index.find(ch) != short_name_index.end())
        {
            build_error("duplicate short option name: " + key);
        }
    }

    void check_add_option_lname(const std::string &key) const
    {
        if (key.empty())
        {
            build_error("long option name cannot be empty");
        }
        if (key.substr(0, 2) != "--")
        {
            build_error("long option name must start with `--`");
        }
        if (find_option_lname(key) != options.cend() || find_sc_option_lname(key) != short_circuit_options.cend())
        {
            build_error("duplicate long option name: " + key);
        }
    }

    template <typename T>
    void check_add_argument_name(const std::string &key) const
    {
        if (type_name<T>() == "null")
        {
            build_error("unsupport type for argument: " + std::string(typeid(T).name()));
        }
        if (type_name<T>() == "bool")
        {
            build_error("bool type argument is not allowed");
        }
        if (key.empty())
        {
            build_error("argument name cannot be empty");
        }
        if (find_argument(key) != arguments.cend() || find_named_argument(key) != named_arguments.cend())
        {
            build_error("duplicate argument name: " + key);
        }
    }
};

}; // namespace util

#endif // JSHL_ARGPARSE_HPP