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
    };


    template<>
    struct CheckReturnValue<CleanupType::kErrorReturn> {
        using ReturnT = Error;
    };


}
