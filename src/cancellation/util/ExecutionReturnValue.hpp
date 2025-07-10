#pragma once

#include "cancellation/CancelType.hpp"

namespace cancellation::util {
    enum class Error : int {
        kSuccess = 0,
        kQueryCancelled,
        kError
    };

    // The values of the specific implementation which suggest what happened after the query is finished/killed, if it is checked via return value rather than exception

    template<CancelType cancel_type>
    struct ReturnValue {
    };

    template<>
    struct ReturnValue<CancelType::kAtomicEnum> {
        using ReturnT = Error;
        static constexpr ReturnT success{Error::kSuccess};
        static constexpr ReturnT cancelled{Error::kQueryCancelled};
        static constexpr ReturnT error{Error::kError};
    };
}
