#include <cstring>
#include <cassert>
#include <zconf.h>
#include <fcntl.h>
#include <iostream>
#include "log_file.h"
#include "timestamp.h"

namespace mona {
LogFile::LogFile(const std::string &dir, const std::string &progressName) :
        logDir_(dir),
        progressName_(progressName),
        logFd_(-1),
        buffers_{0},
        notFilledQueue_(buffNum_),
        filledQueue_(buffNum_),
        currentBufferLen_(0),
        stop_(false) {

    if (!createLogFile()) {
        abort();
    }
    writeFileThread_ = std::thread(&LogFile::writeToFile, this);

    for (size_t i = 0; i < buffNum_; ++i) {
        notFilledQueue_.push(buffers_[i]);
    }
    currentBuffer_ = notFilledQueue_.front();
    notFilledQueue_.pop();
}

void LogFile::append(std::string info) {
    std::lock_guard<std::mutex> guard(currentBufferMutex_);
    assert(info.size() <= bufferSize_);
    if (bufferSize_ - currentBufferLen_ < info.size()) {
        filledQueue_.push(currentBuffer_);
        currentBuffer_ = notFilledQueue_.front();
        notFilledQueue_.pop();
        currentBufferLen_ = 0;
    }
    strcpy(currentBuffer_ + currentBufferLen_, info.c_str());
    currentBufferLen_ += info.size();
}

std::string LogFile::getLogName() {
    std::string result(progressName_);
    char buffer[512] = {0};
    char hostname[128] = {0};
    if (gethostname(hostname, sizeof(hostname) == -1)) {
        strcpy(hostname, "unknown_host");
    }
    snprintf(buffer, sizeof(buffer), ".%s.%s.%d.log", TimeStamp().toLocalTime(true).c_str(),
             hostname, getpid());
    result.append(buffer);
    return result;
}

bool LogFile::createLogFile() {
    std::string logPath(logDir_ + getLogName());
    logFd_ = open(logPath.c_str(), O_CREAT | O_CLOEXEC | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
    if (logFd_ == -1) {
        return false;
    }
    return true;
}

void LogFile::writeToFile() {
    while (true) {
        char *buffer = filledQueue_.front();
        if (buffer == nullptr && stop_) {
            // 表示停止该线程
            break;
        }
        filledQueue_.pop();
        assert(buffer);

        size_t remainLen = strlen(buffer);
        while (remainLen) {
            int len = write(logFd_, buffer, remainLen);
            if (len == -1) {
                abort();
            }
            remainLen -= len;
        }
    }
}

LogFile::~LogFile() {
    filledQueue_.push(currentBuffer_);
    filledQueue_.push(nullptr);
    stop_ = true;
    writeFileThread_.join();
}

void LogFile::flush() {
    std::lock_guard<std::mutex> guard(currentBufferMutex_);
    filledQueue_.push(currentBuffer_);
    currentBuffer_ = notFilledQueue_.front();
    notFilledQueue_.pop();
    currentBufferLen_ = 0;
}
}