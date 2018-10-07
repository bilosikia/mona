#pragma once

namespace mona{

class NoCopyable {
protected:
    NoCopyable() = default;

    ~NoCopyable() = default;

public:
    NoCopyable(const NoCopyable &) = delete;

    NoCopyable &operator=(const NoCopyable &) = delete;
};

}
