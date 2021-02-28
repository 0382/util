#include <cstring>
#include <cwchar>
#include <string>
#include <windows.h>

std::string utf8_to_ansi(const std::string &u8s)
{
    wchar_t *wcs;
    int need_length = MultiByteToWideChar(CP_UTF8, 0, u8s.c_str(), u8s.size(), wcs, 0);
    wcs = new wchar_t[need_length + 1];
    MultiByteToWideChar(CP_UTF8, 0, u8s.c_str(), u8s.size(), wcs, need_length);
    wcs[need_length] = L'\0';

    char *ansi;
    need_length = WideCharToMultiByte(CP_ACP, 0, wcs, std::wcslen(wcs), ansi, 0, nullptr, nullptr);
    ansi = new char[need_length + 1];
    WideCharToMultiByte(CP_ACP, 0, wcs, std::wcslen(wcs), ansi, need_length, nullptr, nullptr);
    ansi[need_length] = '\0';

    std::string result = std::string(ansi);
    delete[] wcs;
    delete[] ansi;
    return result;
}

std::string ansi_to_utf8(const std::string &ansi)
{
    wchar_t *wcs;
    int need_length = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), wcs, 0);
    wcs = new wchar_t[need_length + 1];
    MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), wcs, need_length);
    wcs[need_length] = L'\0';

    char *u8s;
    need_length = WideCharToMultiByte(CP_UTF8, 0, wcs, std::wcslen(wcs), u8s, 0, nullptr, nullptr);
    u8s = new char[need_length + 1];
    WideCharToMultiByte(CP_UTF8, 0, wcs, std::wcslen(wcs), u8s, need_length, nullptr, nullptr);
    u8s[need_length] = '\0';

    std::string result(u8s);
    delete[] wcs;
    delete[] u8s;
    return result;
}
