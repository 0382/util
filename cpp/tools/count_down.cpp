#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

void count_down(const std::string &line)
{
    std::regex pattern(R"((\d{4})-(\d{2})-(\d{2})\s+?(\S+))");
    std::smatch result;
    if (std::regex_search(line, result, pattern))
    {
        std::string date = result[0];
        std::tm tm = {};
        tm.tm_year = std::stoi(result[1]) - 1900;
        tm.tm_mon = std::stoi(result[2]) - 1;
        tm.tm_mday = std::stoi(result[3]);
        tm.tm_isdst = 0;
        std::time_t t = std::mktime(&tm);
        std::tm *ptm = std::localtime(&t);
        auto end_time = std::chrono::system_clock::from_time_t(t);
        auto now_time = std::chrono::system_clock::now();
        auto duration = end_time - now_time;
        auto days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24.0;

        char buffer[16];
        std::strftime(buffer, 16, "%Y-%m-%d", ptm);
        std::string event = result[4];
        std::string message = "还有" + std::to_string(days) + "天到" + event + "(" + std::string(buffer) + ")";
        std::cout << message << std::endl;
    }
    else
    {
        std::cout << "日期格式错误：" << line << std::endl;
    }
}

int main(int argc, char *argv[])
{
    auto config = std::filesystem::path(argv[0]).parent_path() / "count_down.txt";
    std::ifstream fp(config);
    if (!fp.is_open())
    {
        std::cout << "文件打开失败" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(fp, line))
    {
        count_down(line);
    }
    return 0;
}