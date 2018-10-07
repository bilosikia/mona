#pragma once

#include <string>
#include <cassert>
#include "singlton.h"

namespace mona {

class SourceFile {
    std::string name_;
public:
    SourceFile(const std::string &path) {
        assert(!path.empty());
        size_t pos = path.rfind('/');
        if (pos == std::string::npos) {
            name_ = path;
        } else {
            name_ = path.substr(pos + 1);
        }
        assert(!name_.empty());
    }

    std::string name() const {
        assert(!name_.empty());
        return name_;
    }
};

enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS
};

using OutFun =void(const std::string &buffer);
using FlashFun = void();
extern LogLevel g_logLevel;
extern std::function<OutFun> g_outFun;
extern std::function<FlashFun> g_flashFun;

/*
 * log fomat: timestamp.sourcename.linenum.fun.loglevel
 */
class LogStream {
private:
    std::string buffer_;
    bool abort_;

    static std::string logLevelStr(LogLevel level);

public:
    LogStream(LogLevel level, SourceFile sourceFile, int lineNum, std::string func, bool abort);

    LogStream &operator<<(const std::string &info);

    ~LogStream();
};

#define LOG_TRACE if (mona::g_logLevel <= mona::LogLevel::TRACE) \
    mona::LogStream(mona::LogLevel::TRACE, std::string(__FILE__), __LINE__, __func__, false)

#define LOG_DEBUG if (mona::g_logLevel <= mona::LogLevel::DEBUG) \
    mona::LogStream(mona::LogLevel::DEBUG, std::string(__FILE__), __LINE__, __func__, false)

#define LOG_INFO if (mona::g_logLevel <= mona::LogLevel::INFO) \
    mona::LogStream(mona::LogLevel::INFO, std::string(__FILE__), __LINE__, __func__, false)

#define LOG_WARN mona::LogStream(mona::LogLevel::WARN, std::string(__FILE__), __LINE__, __func__, false)
#define LOG_ERROR mona::LogStream(mona::LogLevel::ERROR, std::string(__FILE__), __LINE__, __func__, false)
#define LOG_FATAL mona::LogStream(mona::LogLevel::FATAL, std::string(__FILE__), __LINE__, __func__, true)
}
