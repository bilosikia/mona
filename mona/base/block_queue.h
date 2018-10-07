#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace mona {
template<typename T>
class BoundBlockQueue {
private:
    size_t capacity_;
    std::mutex mutex_;
    std::condition_variable notEmptyCond_;
    std::condition_variable notFullCond_;
    std::queue<T> queue_;

    size_t  queueSize() {
        return queue_.size();
    }

public:
    explicit BoundBlockQueue(int capcity)
            : capacity_(capcity) {
    }

    size_t size() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queueSize();
    }

    int capacity() {
        std::lock_guard<std::mutex> guard(mutex_);
        return capacity_;
    }

    T &front() {
        std::unique_lock<std::mutex> guard(mutex_);
        notEmptyCond_.wait(guard, [this]() {
            return queueSize() > 0;
        });
        return queue_.front();
    }

    void pop() {
        std::unique_lock<std::mutex> guard(mutex_);
        notEmptyCond_.wait(guard, [this]() {
            return queueSize() > 0;
        });
        queue_.pop();
        notFullCond_.notify_all();
    }

    void push(T item) {
        std::unique_lock<std::mutex> guard(mutex_);
        notFullCond_.wait(guard, [this]() {
            return queueSize() < capacity_;
        });
        queue_.push(item);
        notEmptyCond_.notify_all();
    }
};
}