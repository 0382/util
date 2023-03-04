#include "logger.hpp"
#include <thread>

using namespace util;

int main(int argc, char const *argv[])
{
    std::thread th[10];
    auto &log = Logger::instance();
    log.init(LogLevel::Info);
    for (int i = 0; i < 10; ++i)
    {
        th[i] = std::thread(
            [i]()
            {
                for (int k = 0; k < i; ++k)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    log_info("Hello, thread ", i);
                }
            });
    }
    for (int i = 0; i < 10; ++i)
    {
        th[i].join();
    }
    return 0;
}
