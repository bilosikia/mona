#pragma once

#include <mutex>
#include <ctime>
#include <cassert>

namespace mona {
/*
 * year-month-day-minute-second[-microsecond]
 */
class TimeStamp {
private:
    static const int kMicroSecondsPerSecond = 1000 * 1000;
    static std::mutex mutex_;
    std::time_t time_;

public:
    TimeStamp() : time_(std::time(nullptr)) {
    }

    std::time_t timeSinEpoch() const {
        return time_;
    }

    // thread safe
    std::string toLocalTime(bool showMiroSecond = false) const;
};
}

