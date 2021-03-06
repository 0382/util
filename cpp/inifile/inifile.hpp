/**
 * @author:      0.382
 * @description: ini文件读取和修改
 */

#pragma once
#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace util
{

using std::string;
using std::vector;

static constexpr const char *comment_delimiter[] = {
    "#",
    ";",
};

class inifile
{

    // 一个键值对
    struct IniItem
    {
        string key;
        string value;
    };

    // 一个ini段落（以`[]`指示的称为段落）
    struct IniSection
    {
        IniSection(string n) : name(std::move(n)) {}
        string name;
        vector<IniItem> items;

        bool has_key(const string &key) { return find_key(key) != items.cend(); }

        string get_string(const string &key)
        {
            auto pos = check_key(key);
            return pos->value;
        }

        int64_t get_int(const string &key)
        {
            auto pos = check_key(key);
            return std::stoll(pos->value);
        }

        double get_double(const string &key)
        {
            auto pos = check_key(key);
            return std::stod(pos->value);
        }

        bool get_bool(const string &key)
        {
            auto pos = check_key(key);
            string value = to_lower(pos->value);
            if (value == "true" || value == "1")
                return true;
            if (value == "false" || value == "0")
                return false;
            std::cerr << "Section " << this->name << ", key " << key << ", invalid bool value: " << pos->value
                      << std::endl;
            std::exit(-1);
        }

        void set_string(const string &key, const string &value)
        {
            auto pos = find_key(key);
            if (pos == items.end())
            {
                items.push_back(IniItem{key, value});
            }
            else
            {
                pos->value = value;
            }
        }

        void set_int(const string &key, const int64_t &value) { set_value<int64_t>(key, value); }
        void set_double(const string &key, const double &value) { set_value<double>(key, value); }

        void set_bool(const string &key, const bool &value)
        {
            auto pos = find_key(key);
            if (pos == items.end())
            {
                items.push_back(IniItem{key, value ? "true" : "false"});
            }
            else
            {
                pos->value = value ? "true" : "false";
            }
        }

      private:
        using item_iterator = vector<IniItem>::iterator;
        item_iterator find_key(const string &key)
        {
            return std::find_if(items.begin(), items.end(), [&key](IniItem &it) { return it.key == key; });
        }

        item_iterator check_key(const string &key)
        {
            auto pos = find_key(key);
            if (pos == items.end())
            {
                std::cerr << "Section " << this->name << ", key not found: " << key << std::endl;
                std::exit(-1);
            }
            return pos;
        }

        template <typename T>
        void set_value(const string &key, const T &value)
        {
            auto pos = find_key(key);
            if (pos == items.end())
            {
                items.push_back(IniItem{key, std::to_string(value)});
            }
            else
            {
                pos->value = std::to_string(value);
            }
        }
    };

    // 数据
  private:
    vector<IniSection> sections;
    bool _good;
    string msg;

    // 公开函数定义
  public:
    inifile(const string &fname) : _good(true)
    {
        std::fstream inifile(fname);
        if (!inifile.is_open())
        {
            this->_good = false;
            this->msg = "cannot open file: " + fname;
            return;
        }
        string line;
        IniSection sec = {""};
        IniItem item;
        while (!inifile.eof())
        {
            std::getline(inifile, line);
            line = remove_comment(line);
            line = strip(line);
            line = strip(line, '\r');
            if (line == "")
            {
                continue;
            }
            if (line.front() == '[' && line.back() == ']')
            {
                this->sections.push_back(sec);
                sec.name = line.substr(1, line.size() - 2);
                sec.items.clear();
            }
            else if (build_item(line, item))
            {
                sec.items.push_back(item);
            }
            else
            {
                this->_good = false;
                this->msg = "invalid ini line: " + line;
                this->sections.clear();
                return;
            }
        }
        this->sections.push_back(sec);
        inifile.close();
    };

    // ini文件打开是否成功
    bool good() const { return this->_good; }

    // 出错时的错误信息
    string error() const { return this->msg; }

    // 输出到控制台
    void show() const { this->print(std::cout); }

    // 保存到文件
    void save_as(const string &fname) const
    {
        std::ofstream save_file(fname);
        this->print(save_file);
        save_file.close();
    }

    // 获取其中一个 section
    IniSection &section(const string &name = "") { return *check_section(name); }

    // 提供从默认的section中获取value
    string get_string(const string &key) { return section("").get_string(key); }

    int64_t get_int(const string &key) { return section("").get_int(key); }

    double get_double(const string &key) { return section("").get_double(key); }

    bool get_bool(const string &key) { return section("").get_bool(key); }

    void set_string(const string &key, const string &value) { section("").set_string(key, value); }

    void set_int(const string &key, int64_t value) { section("").set_int(key, value); }

    void set_double(const string &key, double value) { section("").set_double(key, value); }

    void set_bool(const string &key, bool value) { section("").set_bool(key, value); }

    // 一些有用的函数
  private:
    // 去除`string`前后的空格（或者其他字符）
    static string strip(const string &line, const char &c = ' ')
    {
        auto p1 = line.find_first_not_of(c);
        if (p1 == string::npos)
            return "";
        auto p2 = line.find_last_not_of(c);
        return line.substr(p1, p2 - p1 + 1);
    }
    // 转换为小写
    static string to_lower(string line)
    {
        for (auto &c : line)
        {
            if (c >= 'A' && c <= 'Z')
            {
                c += 'a' - 'A';
            }
        }
        return line;
    }

    // 去除一行结尾的注释部分
    static string remove_comment(const string &line)
    {
        for (auto com : comment_delimiter)
        {
            auto pos = line.find(com);
            if (pos != string::npos)
            {
                return line.substr(0, pos);
            }
        }
        return line;
    }

    // 从一行构建一个 IniItem
    static bool build_item(const string &line, IniItem &item)
    {
        auto pos = line.find('=');
        if (pos == string::npos)
            return false;
        item.key = strip(line.substr(0, pos));
        item.value = strip(line.substr(pos + 1));
        // 暂时只处理引号，认为没有转义（装死）
        item.value = strip(item.value, '\'');
        item.value = strip(item.value, '"');
        return true;
    }

    // 打印
    void print(std::ostream &os) const
    {
        for (const auto &sec : this->sections)
        {
            if (sec.name != "")
                os << '[' << sec.name << "]\n";
            for (const auto &it : sec.items)
            {
                os << it.key << " = " << it.value << '\n';
            }
        }
    }

    // 查找 section
    vector<IniSection>::iterator check_section(const string &name)
    {
        auto pos =
            std::find_if(sections.begin(), sections.end(), [&name](const IniSection &sec) { return sec.name == name; });
        if (pos == sections.end())
        {
            if (name == "")
                std::cerr << "on default section" << std::endl;
            else
                std::cerr << "section not found: " << name << std::endl;
            std::exit(-1);
        }
        return pos;
    }
};

} // end namespace util

#endif // INIFILE_HPP
