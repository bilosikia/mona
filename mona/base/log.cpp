#include <chrono>
#include <iostream>
#include "log.h"
#include "timestamp.h"

using namespace mona;

namespace mona {
LogLevel g_logLevel = LogLevel::DEBUG;

std::function<OutFun> g_outFun = [](const std::string &buff) -> void {
    std::cout << buff;
    return;
};

std::function<FlashFun> g_flashFun = []() -> void {
    std::cout << std::flush;
    return;
};
}

LogStream::LogStream(LogLevel level, SourceFile sourceFile, int lineNum, std::string func, bool abort)
        : abort_(abort) {
    TimeStamp timestamp;
    char buffer[1024] = {0};
    snprintf(buffer, sizeof(buffer), "%s.%s.%d.%s.%s: ", timestamp.toLocalTime(true).c_str(),
             sourceFile.name().c_str(), lineNum, func.c_str(), logLevelStr(g_logLevel).c_str());
    buffer_ = buffer;
}

LogStream &LogStream::operator<<(const std::string &info) {
    buffer_ += info;
    return *this;
}

LogStream::~LogStream() {
    buffer_.append("\n");
    g_outFun(buffer_);
    if (abort_) {
        g_flashFun();
    }
}

std::string LogStream::logLevelStr(LogLevel level) {
    assert(level >= LogLevel::TRACE && level < LogLevel::NUM_LOG_LEVELS);
    static const char *errs[LogLevel::NUM_LOG_LEVELS] = {"trace", "debug", "info", "warn", "error", "fatal"};
    return errs[level];
}
