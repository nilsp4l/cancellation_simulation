#pragma once
#include <string>
#include "cancellation/ToString.hpp"

namespace cancellation {
    enum class CancelType {
        kAtomicEnum,
        kFunctionExchg,
        kFunctionPointerExchg,
        kInterval,
        kUnion
    };

    template<>
    struct ToString<CancelType, CancelType::kAtomicEnum> {
        static std::string value() { return "Atomic Enum"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kFunctionExchg> {
        static std::string value() { return "Function Exchange"; };
    };

    template<>
    struct ToString<CancelType, CancelType::kFunctionPointerExchg> {
        static std::string value() { return "Function Pointer Exchange"; };
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
