#pragma once

#include <cstdint>

namespace cancellation::util {
    struct Clock {
    public:
        static std::size_t getTimestamp();
    };
}