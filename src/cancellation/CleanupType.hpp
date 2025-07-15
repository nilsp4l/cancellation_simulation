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
        static std::string value(){return "Error Return";};
    };

    template <>
    struct ToString<CleanupType, CleanupType::kException> {
        static std::string value(){return "Exception";};
    };

}