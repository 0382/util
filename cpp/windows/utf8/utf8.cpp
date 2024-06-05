#include <cstring>
#include <cwchar>
#include <iostream>
#include <string>
#include <windows.h>

std::string utf8_to_ansi(const std::string &u8s)
{
    int wcs_len = MultiByteToWideChar(CP_UTF8, 0, u8s.c_str(), u8s.size(), nullptr, 0);
    std::wstring wcs(wcs_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u8s.c_str(), u8s.size(), &wcs[0], wcs_len);

    int ansi_len = WideCharToMultiByte(CP_ACP, 0, wcs.c_str(), wcs.size(), nullptr, 0, nullptr, nullptr);
    std::string ansi(ansi_len, '\0');
    WideCharToMultiByte(CP_ACP, 0, wcs.c_str(), wcs.size(), &ansi[0], ansi_len, nullptr, nullptr);

    return ansi;
}

std::string ansi_to_utf8(const std::string &ansi)
{
    int wcs_len = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), nullptr, 0);
    std::wstring wcs(wcs_len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), &wcs[0], wcs_len);

    int u8s_len = WideCharToMultiByte(CP_UTF8, 0, wcs.c_str(), wcs.size(), nullptr, 0, nullptr, nullptr);
    std::string u8s(u8s_len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wcs.c_str(), wcs.size(), &u8s[0], u8s_len, nullptr, nullptr);

    return u8s;
}

int main()
{
    std::string u8s = "你好，世界！";
    std::string ansi = utf8_to_ansi(u8s);
    std::cout << ansi << std::endl;
    std::string u8s2 = ansi_to_utf8(ansi);
    std::cout << (u8s == u8s2) << std::endl;
    return 0;
}
