#pragma once
#include "ToString.hpp"

namespace cancellation
{
    enum class CleanupType
    {
        kErrorReturn,
        kException
    };

    template <>
    struct ToString<CleanupType, CleanupType::kErrorReturn> {
        static std::string value(){return "ErrorReturn";};
    };

    template <>
    struct ToString<CleanupType, CleanupType::kException> {
        static std::string value(){return "Exception";};
    };

}