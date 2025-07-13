#pragma once
#include <cstdint>
#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/query/Context.hpp"
#include "cancellation/tree/Builder.hpp"
#include <thread>
#include <string>


namespace cancellation::benchmark {


    class Result {
    public:
        Result(const std::string& name, std::size_t cancel_delay_ms,
               const CancelCheckpointRegistry &cancel_checkpoint_registry) :  name_(name), cancel_delay_ms_(
            cancel_delay_ms) {
            auto checkpoints_tps{cancel_checkpoint_registry.getCheckpoints()};
            std::transform(checkpoints_tps.begin(), checkpoints_tps.end(), checkpoints_.begin(), [](auto checkpoint) {
                if (checkpoint) {
                    return std::optional<std::size_t>{checkpoint->time_since_epoch().count()};
                }
                return std::optional<std::size_t>{std::nullopt};
            });
        }


        [[nodiscard]] std::string getName() const {
            return name_;
        }

        [[nodiscard]] std::size_t getCancelDelayMs() const {
            return cancel_delay_ms_;
        }

        auto tuTuple() {
            return std::tuple{
                getName(),
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kExecutionStarted)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kCancelRegistered)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kCancelInitiated)],
                checkpoints_[static_cast<std::size_t>(CancelCheckpointRegistry::Checkpoint::kExecutionFinished)],
                getCancelDelayMs()
            };
        }

    private:
        const std::string name_;
        const std::size_t cancel_delay_ms_;
        std::array<std::optional<std::size_t>, CancelCheckpointRegistry::no_checkpoints> checkpoints_{};
    };


    template<typename implementation, std::size_t size, std::size_t delay>
    class Test {
    public:
        static Result run(std::size_t cancel_delay_ms) {
            CancelCheckpointRegistry cancel_checkpoint_registry;
            query::Context<implementation::cancelType(), implementation::cleanupType()> context{
                &cancel_checkpoint_registry
            };
            auto root{
                tree::Builder<implementation::cancelType(), implementation::cleanupType(), delay, size>::build(&context)
            };
            std::thread cancel_thread{
                [&]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(cancel_delay_ms));
                    cancel_checkpoint_registry.registerCheckpoint(
                        CancelCheckpointRegistry::Checkpoint::kCancelRegistered);
                    context.markInterrupted(util::Error::kQueryCancelled);
                }
            };
            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionStarted);
            if constexpr (implementation::cleanupType() == CleanupType::kException) {
                try {
                    root->next();
                } catch (query::Exception &) {
                }
            } else {
                root->next();
            }
            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionFinished);
            cancel_thread.join();
            return Result{implementation::toString(), cancel_delay_ms, cancel_checkpoint_registry};
        }
    };
}
