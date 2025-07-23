#pragma once

#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/util/CheckReturnValue.hpp"
#include "cancellation/util/ExecutionReturnValue.hpp"
#include "cancellation/query/Exception.hpp"
#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"
#include <atomic>
#include <functional>
#include <mutex>
#include "cancellation/util/Clock.hpp"
#include <iostream>

#include "cancellation/util/Status.hpp"

#define unlikely(x) __builtin_expect(static_cast<bool>(x), 0)

#define NO_CALLING_CONVENTION(__ret_arg, __fun) __asm__ volatile ( \
"call *%1\n"       \
"movl %0, %%eax\n" \
: "=r"(__ret_arg) : "rm"(__fun));

namespace cancellation::query {
    template<CancelType, CleanupType>
    class Context {
    };

    template<CleanupType>
    struct CancelProcedure {
    };



    template<>
    struct CancelProcedure<CleanupType::kException> {
        static constexpr util::CheckReturnValue<CleanupType::kException>::ReturnT execute(
            util::Error error, benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) {
            if (unlikely(error)) {
                cancel_checkpoint_registry->registerCheckpoint(
                    benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                throw Exception{error};
            }
        }

        static constexpr util::CheckReturnValue<CleanupType::kException>::ReturnT execute(
            const util::Status &status, benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) {
            if (unlikely(!status)) {
                cancel_checkpoint_registry->registerCheckpoint(
                    benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                throw Exception{status};
            }
        }
    };

    template<>
    class Context<CancelType::kAtomicEnum, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            error_ = error;
        }

        [[nodiscard]] util::Error checkForInterrupt() const {
            return error_.load();
        }

    private:
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    template<>
    class Context<CancelType::kAtomicEnum, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            error_ = error;
        }

        void checkForInterrupt() {
            if (static_cast<int>(error_.load())) {
                cancel_checkpoint_registry_->registerCheckpoint(benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                throw Exception<util::Error>(error_);
            }
        }

    private:
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };


    template<>
    class Context<CancelType::kFunctionExchg, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_mutex_.lock();
                cancel_function_ = [error, this]() {
                    cancel_checkpoint_registry_->registerCheckpoint(
                        benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    throw Exception{error};
                };
                cancel_mutex_.unlock();
            }
        }

        typename util::CheckReturnValue<CleanupType::kException>::ReturnT checkForInterrupt() {
            cancel_mutex_.lock();
            cancel_function_();
            cancel_mutex_.unlock();
        }

    private:
        std::function<void()> cancel_function_{
            []() {
            }
        };
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;

        std::mutex cancel_mutex_;
    };

    template<>
    class Context<CancelType::kFunctionExchg, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_mutex_.lock();
                cancel_function_ = [error, this]() {
                    cancel_checkpoint_registry_->registerCheckpoint(
                        benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    return error;
                };
                cancel_mutex_.unlock();
            }
        }

        typename util::CheckReturnValue<CleanupType::kErrorReturn>::ReturnT checkForInterrupt() {
            cancel_mutex_.lock();
            auto ret = cancel_function_();
            cancel_mutex_.unlock();
            return ret;
        }

    private:
        std::function<util::Error()> cancel_function_{
            []() {
                return util::Error::kSuccess;
            }
        };
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;

        std::mutex cancel_mutex_;
    };





    template<>
    class Context<CancelType::kFunctionPointerExchg, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }



        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_function_ = &Context::cancel;
            }
        }


        typename util::CheckReturnValue<CleanupType::kException>::ReturnT checkForInterrupt() {
            std::invoke(cancel_function_.load(), this);
        }


    private:

        void cancel() {
            cancel_checkpoint_registry_->registerCheckpoint(
                benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
            throw Exception{util::Error::kQueryCancelled};
        }
        void nop() {
        }

        typedef void (Context::*CancelFunc)();

        std::atomic<CancelFunc> cancel_function_{&Context::nop};

        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    static util::Error returnSuccess() {
        return util::Error::kSuccess;
    }

    static util::Error returnCancelled() {
        return util::Error::kQueryCancelled;
    }

    template<>
    class Context<CancelType::kFunctionPointerExchg, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_function_ = returnCancelled;
            }
        }


        typename util::CheckReturnValue<CleanupType::kErrorReturn>::ReturnT checkForInterrupt() {
            return (*cancel_function_)();
        }

    private:
        typedef util::Error (*CancelFunc)();

        std::atomic<CancelFunc> cancel_function_ = returnSuccess;

        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    // only works for error return
    template<>
    class Context<CancelType::kFunctionPointerExchgCallConv, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_function_ = returnCancelled;
            }
        }


        typename util::CheckReturnValue<CleanupType::kErrorReturn>::ReturnT __attribute__((noinline)) checkForInterrupt() {
            volatile int ret;
            NO_CALLING_CONVENTION(ret, cancel_function_.load());
            return static_cast<util::Error>(ret);
        }

    private:
        typedef util::Error (*CancelFunc)();

        std::atomic<CancelFunc> cancel_function_ = returnSuccess;

        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    template<>
    class Context<CancelType::kInterval, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
            next_check_ = util::Clock::getTimestamp() + interval_ticks;
        }

        static constexpr std::size_t interval_ticks{50'000'000};

        void markInterrupted(util::Error error) {
            error_ = error;
        }

        util::Error checkForInterrupt() {

            if (const auto now = util::Clock::getTimestamp(); now >= next_check_) {
                next_check_ = now + interval_ticks;


                return error_;
            }
            return util::CheckReturnValue<CleanupType::kErrorReturn>::not_cancelled();
        }

    private:
        std::size_t next_check_;
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };

    template<>
    class Context<CancelType::kInterval, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
            next_check_ = util::Clock::getTimestamp() + interval_ticks;
        }

        static constexpr std::size_t interval_ticks{50'000'000};

        void markInterrupted(util::Error error) {
            error_ = error;
        }

        void checkForInterrupt() {

            if (const auto now = util::Clock::getTimestamp(); now >= next_check_) {
                next_check_ = now + interval_ticks;

                if (static_cast<bool>(error_.load())) {
                    cancel_checkpoint_registry_->registerCheckpoint(benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    throw Exception(error_.load());
                }

            }
        }

    private:
        std::size_t next_check_;
        std::atomic<util::Error> error_{util::Error::kSuccess};
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
    };


    template<>
    class Context<CancelType::kUnion, CleanupType::kErrorReturn> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_mutex_.lock();
                status_ = util::Status::Error{"cancelled"};
                cancel_mutex_.unlock();
            }
        }

        util::Error checkForInterrupt() {

            cancel_mutex_.lock();
            auto return_value{status_ ? util::Error::kSuccess : util::Error::kQueryCancelled};
            cancel_mutex_.unlock();
            return return_value;

        }

    private:
        util::Status status_;
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
        std::mutex cancel_mutex_;
    };

    template<>
    class Context<CancelType::kUnion, CleanupType::kException> {
    public:
        explicit Context(benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry) : cancel_checkpoint_registry_(
            cancel_checkpoint_registry) {
        }

        void markInterrupted(util::Error error) {
            if (static_cast<bool>(error)) {
                cancel_mutex_.lock();
                status_ = util::Status::Error{"cancelled"};
                cancel_mutex_.unlock();
            }
        }

        void checkForInterrupt() {

                cancel_mutex_.lock();
                if (!status_) {
                    cancel_checkpoint_registry_->registerCheckpoint(benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated);
                    throw Exception(status_);
                }
                cancel_mutex_.unlock();

        }

    private:
        util::Status status_;
        benchmark::CancelCheckpointRegistry *cancel_checkpoint_registry_;
        std::mutex cancel_mutex_;
    };
}
