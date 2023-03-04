#pragma once
#ifndef UTIL_LOGGER_HPP
#define UTIL_LOGGER_HPP

#include <ctime>
#include <iostream>
#include <sstream>

namespace util
{

enum class LogLevel
{
    Error = 0,
    Warning = 1,
    Info = 2,
    Debug = 3,
    Verbose = 4
};

class Logger
{
  public:
    static Logger &instance()
    {
        static Logger log;
        return log;
    }

    void init(LogLevel level)
    {
        m_log_level = level;
        std::time(&m_start_time);
    }

    template <typename... Args>
    void error(Args &&...args) const
    {
        write_log(LogLevel::Error, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error_stop(Args &&...args) const
    {
        write_log(LogLevel::Error, std::forward<Args>(args)...);
        std::exit(-1);
    }

    template <typename... Args>
    void warn(Args &&...args) const
    {
        write_log(LogLevel::Warning, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(Args &&...args) const
    {
        write_log(LogLevel::Info, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(Args &&...args) const
    {
        write_log(LogLevel::Debug, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void verbose(Args &&...args) const
    {
        write_log(LogLevel::Verbose, std::forward<Args>(args)...);
    }

  private:
    LogLevel m_log_level;
    std::time_t m_start_time;

  private:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;

    template <typename... Args>
    void write_log(LogLevel level, Args &&...args) const
    {
        static thread_local char tm_buffer[16];
        if (static_cast<int>(level) > static_cast<int>(m_log_level))
            return;
        std::time_t now_time = std::time(nullptr);
        std::time_t diff_time = int(std::difftime(now_time, m_start_time));
        std::strftime(tm_buffer, sizeof(tm_buffer), "[%H:%M:%S] ", std::gmtime(&diff_time));
        std::ostringstream oss;
        oss << tm_buffer;
        (oss << ... << args) << '\n';
        std::cout << oss.str();
        std::cout.flush();
    }
};

template <typename... Args>
inline void log_error(Args &&...args)
{
    Logger::instance().error(std::forward<Args>(args)...);
}

template <typename... Args>
inline void log_error_stop(Args &&...args)
{
    Logger::instance().error_stop(std::forward<Args>(args)...);
}

template <typename... Args>
inline void log_warn(Args &&...args)
{
    Logger::instance().warn(std::forward<Args>(args)...);
}

template <typename... Args>
inline void log_info(Args &&...args)
{
    Logger::instance().info(std::forward<Args>(args)...);
}

template <typename... Args>
inline void log_debug(Args &&...args)
{
    Logger::instance().debug(std::forward<Args>(args)...);
}

template <typename... Args>
inline void log_verbose(Args &&...args)
{
    Logger::instance().verbose(std::forward<Args>(args)...);
}

} // end namespace util

#endif // UTIL_LOGGER_HPP