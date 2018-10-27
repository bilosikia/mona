#include <iostream>
#include "mona/base/singlton.h"
#include "mona/base/timestamp.h"
#include "mona/base/log.h"
#include "mona/base/block_queue.h"
#include "mona/base/log_file.h"
#include "mona/base/errno_str.h"

int main() {
    std::string errs = mona::errnoToStr(EINTR, "sb");

    mona::LogFile logFile("log", "main");
    mona::g_outFun = [&logFile](const std::string &buffer) -> void {
        logFile.append(buffer);
        return;
    };
    mona::g_flashFun = [&logFile]() {
        logFile.flush();
    };
    mona::g_logLevel = mona::LogLevel::DEBUG;

    {
        LOG_TRACE << "HELLO WORLD" << "WHAT IS UP";
        LOG_DEBUG << "HELLO WORLD";
        LOG_INFO << "HELLO WORLD";
        LOG_WARN << "HELLO WORLD";
        LOG_ERROR << "HELLO WORLD";
//        LOG_FATAL << "HELLO WORLD";
    }
    return 0;
}