#pragma once

#include <memory>
#include <mutex>
#include "nocopyable.h"

namespace mona{

template<typename T>
class Singlton : public NoCopyable {
protected:
    static std::shared_ptr<T> instance_;
    static std::once_flag onceFlag_;

    Singlton() = default;

    ~Singlton() = default;

public:
    static T &instance() {
        std::call_once(onceFlag_, []() {
            instance_ = std::make_shared<T>();
        });
        return *instance_;
    }
};

template<typename T>
std::shared_ptr<T> Singlton<T>::instance_;

template<typename T>
std::once_flag Singlton<T>::onceFlag_;
}
