#pragma once

#include <string>
#include <array>
#include <thread>
#include <atomic>
#include "nocopyable.h"
#include "block_queue.h"

namespace mona {

class LogFile: public NoCopyable {
private:
    static const size_t bufferSize_ = 512;
    static  const size_t buffNum_ = 4;

    std::string logDir_;
    std::string progressName_;
    int logFd_;
    char buffers_[buffNum_][bufferSize_];
    BoundBlockQueue<char *> notFilledQueue_;
    BoundBlockQueue<char *> filledQueue_;
    char *currentBuffer_;
    size_t currentBufferLen_;
    std::mutex currentBufferMutex_;
    std::thread writeFileThread_;
    std::atomic<bool> stop_;

    bool rollFile();
    std::string getLogName();
    bool createLogFile();
    void writeToFile();
public:
    LogFile(const std::string &dir, const std::string &progressName);
    ~LogFile();
    void flush();
    void append(std::string info);

};
}

