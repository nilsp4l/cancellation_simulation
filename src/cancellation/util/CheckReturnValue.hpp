#pragma once

#include "ExecutionReturnValue.hpp"
#include "cancellation/CleanupType.hpp"

namespace cancellation::util {
    template<CleanupType cleanup_type>
    struct CheckReturnValue {
    };

    template<>
    struct CheckReturnValue<CleanupType::kException> {
        using ReturnT = void;
        static constexpr ReturnT cancelled() { return void(); };
        static constexpr ReturnT not_cancelled() { return void(); };
    };


    template<>
    struct CheckReturnValue<CleanupType::kErrorReturn> {
        using ReturnT = Error;
        static constexpr ReturnT cancelled() { return ReturnT::kQueryCancelled; };
        static constexpr ReturnT not_cancelled() { return ReturnT::kSuccess; };
    };
}
