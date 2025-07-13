#pragma once
#include <string>
#include "cancellation/util/ToString.hpp"

namespace cancellation
{
    enum class CancelType
    {
        kAtomicEnum,
        kFunctionExchg,
        kFunctionPointerExchg,
        kInterval
    };
    template <>
        struct util::ToString<CancelType, CancelType::kAtomicEnum> {
        static std::string value(){return "Atomic Enum";};
    };

    template <>
    struct util::ToString<CancelType, CancelType::kFunctionExchg> {
        static std::string value(){return "Function Exchange";};
    };

    template <>
    struct util::ToString<CancelType, CancelType::kFunctionPointerExchg> {
        static std::string value(){return "Function Pointer Exchange";};
    };


    template <>
    struct util::ToString<CancelType, CancelType::kInterval> {
        static std::string value(){return "Interval";};
    };

}