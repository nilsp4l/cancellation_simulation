#pragma once

#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/util/CheckReturnValue.hpp"
#include "cancellation/util/ExecutionReturnValue.hpp"
#include "cancellation/query/Exception.hpp"
#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"
#include <atomic>
#include <functional>

namespace cancellation::query {
    template<CancelType, CleanupType>
    class Context {
    };

    template<CleanupType>
    struct CancelProcedure {
    };

    template<>
    struct CancelProcedure<CleanupType::kErrorReturn> {
        static constexpr util::CheckReturnValue<CleanupType::kErrorReturn>::ReturnT execute(
            util::Error error, benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) {
            if (static_cast<bool>(error)) {
                cancel_checkpoint_registry->registerCheckpoint(
                    benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                return error;
            }
            return util::Error::kSuccess;
        }
    };

    template<>
    struct CancelProcedure<CleanupType::kException> {
        static constexpr util::CheckReturnValue<CleanupType::kException>::ReturnT execute(
            util::Error error, benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) {
            if (static_cast<bool>(error)) {
                cancel_checkpoint_registry->registerCheckpoint(
                    benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                throw Exception{error};
            }
        }
    };

    template<CleanupType cleanup_type>
    class Context<CancelType::kAtomicEnum, cleanup_type> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            error_ = error;
        }

        typename util::CheckReturnValue<cleanup_type>::ReturnT checkForInterrupt() {
            return CancelProcedure<cleanup_type>::execute(error_.load(), cancel_checkpoint_registry_);
        }

    private:
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    // this approach only makes sens with an exception thrown
    template<>
    class Context<CancelType::kFunctionExchg, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_function_ = [error, this]() {
                    cancel_checkpoint_registry_->registerCheckpoint(
                        benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    throw Exception{error};
                };
            }
        }

        typename util::CheckReturnValue<CleanupType::kException>::ReturnT checkForInterrupt() {
            cancel_function_();
        }

    private:
        std::function<void()> cancel_function_{
            []() {
            }
        };
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    template<CleanupType cleanup_type>
    class Context<CancelType::kInterval, cleanup_type> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
            next_check_ = std::chrono::steady_clock::now().time_since_epoch().count() + interval_ms;
        }
        static constexpr std::size_t interval_ms{10};
        void markInterrupted(util::Error error) {
            error_ = error;
        }

        typename util::CheckReturnValue<cleanup_type>::ReturnT checkForInterrupt() {
            if (const auto now = std::chrono::steady_clock::now().time_since_epoch().count(); now >= next_check_) {
                next_check_ = now + interval_ms;
                return CancelProcedure<cleanup_type>::execute(error_.load(), cancel_checkpoint_registry_);
            }
            return util::CheckReturnValue<cleanup_type>::not_cancelled();

        }

    private:
        std::size_t next_check_;
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;

    };

}
