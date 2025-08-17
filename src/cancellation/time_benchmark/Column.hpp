#pragma once

#include <optional>
#include <cstdint>
#include "cancellation/time_benchmark/ToString.hpp"


namespace cancellation::time_benchmark {
    enum class ColumnEnum : std::size_t {
        kCancelType = 0,
        kCleanupType,
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
    struct ColumnType<ColumnEnum::kCancelType> {
        using type = std::string;
    };

    template<>
    struct ColumnType<ColumnEnum::kCleanupType> {
        using type = std::string;
    };



    template<>
    struct ToString<ColumnEnum, ColumnEnum::kCancelType> {
        static std::string value() {
            return "CancelType";
        }
    };

    template<>
    struct ToString<ColumnEnum, ColumnEnum::kCleanupType> {
        static std::string value() {
            return "CleanupType";
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