#include "timestamp.h"

namespace mona {
const int TimeStamp::kMicroSecondsPerSecond;
std::mutex TimeStamp::mutex_;

std::string TimeStamp::toLocalTime(bool showMiroSecond) const {
    std::lock_guard<std::mutex> mutexGuard(TimeStamp::mutex_);
    std::tm *tm = std::localtime(&time_);
    assert(tm);
    char buff[512] = {0};
    if (!showMiroSecond) {
        snprintf(buff, sizeof(buff), "%d-%d-%d-%d-%d-%d", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday,
                 tm->tm_hour, tm->tm_min, tm->tm_sec);
    } else {
        snprintf(buff, sizeof(buff), "%d-%d-%d-%d-%d-%d-%ld", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday,
                 tm->tm_hour, tm->tm_min, tm->tm_sec, time_ % kMicroSecondsPerSecond);
    }
    return buff;
}
}
