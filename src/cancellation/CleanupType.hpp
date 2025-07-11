#pragma once
#include "cancellation/util/ToString.hpp"

namespace cancellation
{
    enum class CleanupType
    {
        kErrorReturn,
        kException
    };

    template <>
    struct util::ToString<CleanupType, CleanupType::kErrorReturn> {
        static std::string value(){return "Error Return";};
    };

    template <>
    struct util::ToString<CleanupType, CleanupType::kException> {
        static std::string value(){return "Exception";};
    };

}