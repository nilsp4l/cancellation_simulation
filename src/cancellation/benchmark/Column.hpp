#pragma once

#include <optional>
#include <cstdint>
#include "cancellation/benchmark/ToString.hpp"


namespace cancellation::benchmark {
    enum class ColumnEnum : std::size_t {
        kName = 0,
        kExecutionStarted,
        kRegistered,
        kCancelInitiated,
        kExecutionFinished,
        kCancelDelayMs,
        size
    };

    template<ColumnEnum>
    struct ColumnType {
        using type = std::optional<std::size_t>;
    };

    template<>
    struct ColumnType<ColumnEnum::kName> {
        using type = std::string;
    };



    template<>
    struct ToString<ColumnEnum, ColumnEnum::kName> {
        static std::string value() {
            return "Name";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kExecutionStarted> {
        static std::string value() {
            return "ExecutionStarted";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kRegistered> {
        static std::string value() {
            return "CancelRegistered";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kCancelInitiated> {
        static std::string value() {
            return "CancelInitiated";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kExecutionFinished> {
        static std::string value() {
            return "ExecutionFinished";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kCancelDelayMs> {
        static std::string value() {
            return "CancelDelayMs";
        }
    };


}