#pragma once
#ifndef WINDOWS_SEARCH_HPP
#define WINDOWS_SEARCH_HPP

#include <iostream>
#include <regex>
#include <string>
#include <windows.h>

// 判断找到的文件是否等于 "." 或者 ".."
bool is_trivial_diractory(WIN32_FIND_DATAA &file)
{
    std::string_view dir = file.cFileName;
    return (dir == ".") || (dir == "..");
}

// 判断是否是文件夹
bool is_directory(WIN32_FIND_DATAA &file)
{
    return file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

// 打印正则匹配结果
std::ostream &operator<<(std::ostream &os, const std::smatch &sm)
{
    os << sm.prefix();
    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hd, FOREGROUND_RED);
    os << sm.str();
    SetConsoleTextAttribute(hd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    os << sm.suffix();
    return os;
}

// 使用正则匹配搜索
void search_file_with_regex(const std::string &dir_name, const std::regex &search_regex)
{
    WIN32_FIND_DATAA current_file;
    std::string search_dir = dir_name + "\\*";
    std::string file_name;
    std::smatch sm;
    HANDLE search_handle = FindFirstFileA(search_dir.c_str(), &current_file);
    if (search_handle != INVALID_HANDLE_VALUE)
    {
        if (!is_trivial_diractory(current_file))
        {
            file_name = std::string(current_file.cFileName);
            if (std::regex_search(file_name, sm, search_regex))
            {
                std::cout << dir_name << "\\" << sm << std::endl;
            }
            if (is_directory(current_file))
            {
                search_file_with_regex(dir_name + "\\" + current_file.cFileName, search_regex);
            }
        }
        while (FindNextFileA(search_handle, &current_file))
        {
            if (is_trivial_diractory(current_file))
                continue;
            file_name = std::string(current_file.cFileName);
            if (std::regex_search(file_name, sm, search_regex))
            {
                std::cout << dir_name << "\\" << sm << std::endl;
            }
            if (is_directory(current_file))
            {
                search_file_with_regex(dir_name + "\\" + current_file.cFileName, search_regex);
            }
        }
    }
    FindClose(search_handle);
}

#endif // WINDOWS_SEARCH_HPP