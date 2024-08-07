/**
 * @author: 0.382
 * @description: ini文件读取和修改
 * @url: https://github.com/0382/util/tree/main/cpp/inifile
 */

#pragma once
#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace util
{
// ini 文件注释符号
static constexpr const char *ini_comment_delimiter[] = {
    "#",
    ";",
};

class inifile
{
    // 一个键值对
    struct IniItem
    {
        std::string key;
        std::string value;
    };

    // 一个ini段落（以`[]`指示的称为段落）
    struct IniSection
    {
      private:
        std::string secname;
        std::vector<IniItem> items;

        static std::string show_name(const std::string &secname)
        {
            if (secname.empty())
                return "*DEFAULT*";
            return secname;
        }

      public:
        friend class inifile;
        using item_iterator = std::vector<IniItem>::iterator;
        using item_const_iterator = std::vector<IniItem>::const_iterator;

        IniSection(std::string name) : secname(std::move(name)) {}

        // 迭代器
        item_iterator begin() { return items.begin(); }
        item_iterator end() { return items.end(); }
        item_const_iterator begin() const { return items.cbegin(); }
        item_const_iterator end() const { return items.cend(); }
        item_const_iterator cbegin() const { return items.cbegin(); }
        item_const_iterator cend() const { return items.cend(); }

        // 段落名
        std::string name() const { return show_name(secname); }

        bool has_key(const std::string &key) const { return find_key(key) != items.cend(); }

        std::string get_string(const std::string &key) const
        {
            auto pos = check_key(key);
            return pos->value;
        }

        int64_t get_int(const std::string &key) const
        {
            auto pos = check_key(key);
            return std::stoll(pos->value);
        }

        double get_double(const std::string &key) const
        {
            auto pos = check_key(key);
            return std::stod(pos->value);
        }

        bool get_bool(const std::string &key) const
        {
            auto pos = check_key(key);
            std::string value = to_lower(pos->value);
            if (value == "true" || value == "1")
                return true;
            if (value == "false" || value == "0")
                return false;
            std::cerr << "Section " << this->name() << ", key " << key << ", invalid bool value: " << pos->value
                      << std::endl;
            std::exit(-1);
        }

        void set_string(const std::string &key, const std::string &value)
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

        void set_int(const std::string &key, int64_t value) { set_value<int64_t>(key, value); }
        void set_double(const std::string &key, double value) { set_value<double>(key, value); }

        void set_bool(const std::string &key, bool value)
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
        item_iterator find_key(const std::string &key)
        {
            return std::find_if(items.begin(), items.end(), [&key](const IniItem &it) { return it.key == key; });
        }

        item_const_iterator find_key(const std::string &key) const
        {
            return std::find_if(items.cbegin(), items.cend(), [&key](const IniItem &it) { return it.key == key; });
        }

        item_iterator check_key(const std::string &key)
        {
            auto pos = find_key(key);
            if (pos == items.end())
            {
                std::cerr << "Section " << this->name() << ", key not found: " << key << std::endl;
                std::exit(-1);
            }
            return pos;
        }

        item_const_iterator check_key(const std::string &key) const
        {
            auto pos = find_key(key);
            if (pos == items.cend())
            {
                std::cerr << "Section " << this->name() << ", key not found: " << key << std::endl;
                std::exit(-1);
            }
            return pos;
        }

        template <typename T>
        void set_value(const std::string &key, const T &value)
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
    std::vector<IniSection> sections;
    bool _good;
    std::string msg;

    // 公开函数定义
  public:
    inifile(const std::string &fname) : _good(true)
    {
        std::ifstream fp(fname);
        if (!fp.is_open())
        {
            this->_good = false;
            this->msg = "open file failed: " + fname;
            return;
        }
        std::string line;
        IniSection sec = {""};
        IniItem item;
        while (!fp.eof())
        {
            std::getline(fp, line);
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
                sec.secname = line.substr(1, line.size() - 2);
                if (has_section(sec.secname))
                {
                    this->_good = false;
                    this->msg = "duplicate section: " + sec.secname;
                    return;
                }
                sec.items.clear();
            }
            else if (build_item(line, item))
            {
                if (sec.has_key(item.key))
                {
                    this->_good = false;
                    this->msg = "duplicate key: " + item.key + " in section " + sec.name();
                    return;
                }
                sec.items.push_back(item);
            }
            else
            {
                this->_good = false;
                this->msg = "invalid ini line: " + line;
                return;
            }
        }
        this->sections.push_back(sec);
    }

    // 迭代器
    std::vector<IniSection>::iterator begin() { return sections.begin(); }
    std::vector<IniSection>::iterator end() { return sections.end(); }
    std::vector<IniSection>::const_iterator begin() const { return sections.cbegin(); }
    std::vector<IniSection>::const_iterator end() const { return sections.cend(); }
    std::vector<IniSection>::const_iterator cbegin() const { return sections.cbegin(); }
    std::vector<IniSection>::const_iterator cend() const { return sections.cend(); }

    // ini文件打开是否成功
    bool good() const { return this->_good; }

    // 出错时的错误信息
    std::string error_msg() const { return this->msg; }

    // 输出到控制台
    void show() const { this->print(std::cout); }

    // 保存到文件
    void save_as(const std::string &fname) const
    {
        std::ofstream save_file(fname);
        this->print(save_file);
        save_file.close();
    }

    bool has_section(const std::string &name) const { return find_section(name) != sections.cend(); }

    // 获取其中一个 section
    IniSection &section(const std::string &name) { return *check_section(name); }
    const IniSection &section(const std::string &name) const { return *check_section(name); }

    inifile &add_section(const std::string &name)
    {
        if (has_section(name))
        {
            std::cerr << "duplicate section: " << IniSection::show_name(name) << std::endl;
            std::exit(-1);
        }
        this->sections.push_back(IniSection{name});
        return *this;
    }

    // 提供从默认的section中获取value
    std::string get_string(const std::string &key) const { return section("").get_string(key); }
    int64_t get_int(const std::string &key) const { return section("").get_int(key); }
    double get_double(const std::string &key) const { return section("").get_double(key); }
    bool get_bool(const std::string &key) const { return section("").get_bool(key); }

    // 提供设置默认section的value的方法
    void set_string(const std::string &key, const std::string &value) { section("").set_string(key, value); }
    void set_int(const std::string &key, int64_t value) { section("").set_int(key, value); }
    void set_double(const std::string &key, double value) { section("").set_double(key, value); }
    void set_bool(const std::string &key, bool value) { section("").set_bool(key, value); }

    // 一些有用的函数
  private:
    // 去除`string`前后的空格（或者其他字符）
    static std::string strip(const std::string &line, char c = ' ')
    {
        auto p1 = line.find_first_not_of(c);
        if (p1 == std::string::npos)
            return "";
        auto p2 = line.find_last_not_of(c);
        return line.substr(p1, p2 - p1 + 1);
    }
    // 转换为小写
    static std::string to_lower(std::string line)
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
    static std::string remove_comment(const std::string &line)
    {
        for (auto com : ini_comment_delimiter)
        {
            auto pos = line.find(com);
            if (pos != std::string::npos)
            {
                return line.substr(0, pos);
            }
        }
        return line;
    }

    // 从一行构建一个 IniItem
    static bool build_item(const std::string &line, IniItem &item)
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
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
            if (!sec.secname.empty())
                os << '[' << sec.secname << "]\n";
            for (const auto &it : sec.items)
            {
                os << it.key << " = " << it.value << '\n';
            }
        }
    }

    std::vector<IniSection>::iterator find_section(const std::string &name)
    {
        return std::find_if(sections.begin(), sections.end(),
                            [&name](const IniSection &sec) { return sec.secname == name; });
    }

    std::vector<IniSection>::const_iterator find_section(const std::string &name) const
    {
        return std::find_if(sections.cbegin(), sections.cend(),
                            [&name](const IniSection &sec) { return sec.secname == name; });
    }

    // 查找 section
    std::vector<IniSection>::iterator check_section(const std::string &name)
    {
        auto pos = find_section(name);
        if (pos == sections.end())
        {
            std::cerr << "section not found: " << IniSection::show_name(name) << std::endl;
            std::exit(-1);
        }
        return pos;
    }

    std::vector<IniSection>::const_iterator check_section(const std::string &name) const
    {
        auto pos = find_section(name);
        if (pos == sections.cend())
        {
            std::cerr << "section not found: " << IniSection::show_name(name) << std::endl;
            std::exit(-1);
        }
        return pos;
    }
};

} // end namespace util

#endif // INIFILE_HPP
