#pragma once

#include "cancellation/time_benchmark/CancelCheckpointRegistry.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/query/Context.hpp"
#include "cancellation/tree/Builder.hpp"
#include "cancellation/util/ExceptionReturnValue.hpp"
#include "cancellation/CancelType.hpp"
#include <thread>
#include <string>


namespace cancellation::time_benchmark {
    class Result {
    public:
        Result(std::string cancel_type_name, std::string cleanup_type_name, std::optional<std::size_t> cancel_delay_ms,
               const CancelCheckpointRegistry &cancel_checkpoint_registry) : cancel_type_name_(
                                                                                 std::move(cancel_type_name)),
                                                                             cleanup_type_name_(
                                                                                 std::move(cleanup_type_name)),
                                                                             cancel_delay_ms_(
                                                                                 cancel_delay_ms) {
            auto checkpoints_tps{cancel_checkpoint_registry.getCheckpoints()};
            std::transform(checkpoints_tps.begin(), checkpoints_tps.end(), checkpoints_.begin(), [](auto checkpoint) {
                if (checkpoint) {
                    return std::optional<std::size_t>{checkpoint->time_since_epoch().count()};
                }
                return std::optional<std::size_t>{std::nullopt};
            });
        }


        [[nodiscard]] std::string getCancelTypeName() const {
            return cancel_type_name_;
        }

        [[nodiscard]] std::string getCleanupTypeName() const {
            return cleanup_type_name_;
        }

        [[nodiscard]] std::optional<std::size_t> getCancelDelayMs() const {
            return cancel_delay_ms_;
        }

        auto tuTuple() {
            return std::tuple{
                getCancelTypeName(),
                getCleanupTypeName(),
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kExecutionStarted)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kCancelRegistered)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kCancelInitiated)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kExecutionFinished)],
                getCancelDelayMs()
            };
        }

    private:
        const std::string cancel_type_name_;
        const std::string cleanup_type_name_;
        const std::optional<std::size_t> cancel_delay_ms_;
        std::array<std::optional<std::size_t>, CancelCheckpointRegistry::no_checkpoints> checkpoints_{};
    };


    template<typename implementation, std::size_t size, std::size_t delay>
    class Test {
    public:
        static Result run(std::optional<std::size_t> cancel_delay_ms) {
            CancelCheckpointRegistry cancel_checkpoint_registry;
            query::Context<implementation::cancelType(), implementation::cleanupType()> context{
                &cancel_checkpoint_registry
            };
            auto root{
                tree::Builder<implementation::cancelType(), implementation::cleanupType(), delay, size>::build(&context, &cancel_checkpoint_registry)
            };
            std::unique_ptr<std::thread> cancel_thread;
            if (cancel_delay_ms) {
                cancel_thread = std::make_unique<std::thread>(
                    [&]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(*cancel_delay_ms));
                        cancel_checkpoint_registry.registerCheckpoint(
                            CancelCheckpointRegistry::Checkpoint::kCancelRegistered);
                        context.markInterrupted(util::Error::kQueryCancelled);
                    }
                );
            }

            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionStarted);
            if constexpr (implementation::cleanupType() == CleanupType::kException) {
                try {
                    while (!root->next());
                } catch (query::Exception<typename util::ExceptionReturnValue<implementation::cancelType()>::type> &) {
                }
            } else {
                while (!root->next());
            }
            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionFinished);
            if (cancel_thread) {
                cancel_thread->join();
            }

            return Result{
                ::cancellation::ToString<CancelType, implementation::cancelType()>::value(),
                ::cancellation::ToString<CleanupType, implementation::cleanupType()>::value(), cancel_delay_ms, cancel_checkpoint_registry
            };
        }
    };
}
