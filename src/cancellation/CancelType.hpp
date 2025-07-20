#pragma once
#include <string>
#include "cancellation/ToString.hpp"

namespace cancellation {
    enum class CancelType {
        kAtomicEnum,
        kFunctionExchg,
        kFunctionPointerExchg,
        kFunctionPointerExchgCallConv,
        kInterval,
        kUnion
    };

    template<>
    struct ToString<CancelType, CancelType::kAtomicEnum> {
        static std::string value() { return "AtomicEnum"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kFunctionExchg> {
        static std::string value() { return "FunctionExchange"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kFunctionPointerExchg> {
        static std::string value() { return "FunctionPointerExchange"; };
    };

    template <>
    struct ToString<CancelType, CancelType::kFunctionPointerExchgCallConv> {
        static std::string value() { return "FunctionPointerExchangeCallConv"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kInterval> {
        static std::string value() { return "Interval"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kUnion> {
        static std::string value() { return "Union"; };
    };
}
