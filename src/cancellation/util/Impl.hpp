#pragma once

#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/util/ToString.hpp"

namespace cancellation::util {
    template<CancelType cancel_type, CleanupType cleanup_type>
    struct Impl {
        static constexpr CancelType cancelType() { return cancel_type; }
        static constexpr CleanupType cleanupType() { return cleanup_type; }

        static std::string toString() {
            return "[" + ToString<CancelType, cancel_type>::value() + ", " + ToString<CleanupType,
                       cleanup_type>::value() + "]";
        }
    };
}
