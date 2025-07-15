#pragma once


#include "cancellation/util/ExecutionReturnValue.hpp"
#include "cancellation/util/Status.hpp"
#include "cancellation/CancelType.hpp"

namespace cancellation::util {
    template <CancelType cancel_type>
    struct ExceptionReturnValue {
        using type = Error;
    };

    template <>
    struct ExceptionReturnValue<CancelType::kUnion> {
        using type = Status;
    };
}


