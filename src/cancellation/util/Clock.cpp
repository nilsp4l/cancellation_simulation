#include "cancellation/util/Clock.hpp"

namespace cancellation::util {
    std::size_t Clock::getTimestamp() {
        unsigned int lower;
        unsigned int upper;
        __asm volatile("rdtsc" : "=a"(lower), "=d"(upper));
        return (static_cast<std::size_t>(upper) << 32) | static_cast<std::size_t>(lower);
    }

}