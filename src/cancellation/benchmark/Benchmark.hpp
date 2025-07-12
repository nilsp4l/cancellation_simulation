#pragma once

#include "cancellation/benchmark/Position.hpp"
#include "cancellation/tree/Node.hpp"
#include "cancellation/tree/Builder.hpp"
#include "cancellation/query/Context.hpp"
#include <algorithm>

namespace cancellation::benchmark {
    template <std::size_t size, std::size_t delay, typename... implementations>
    struct Benchmark {
    public:
        static constexpr std::array<std::unique_ptr<Position<CancelCheckpointRegistry::no_checkpoints>>, sizeof...(implementations)>  run() {
            std::array<std::unique_ptr<Position<CancelCheckpointRegistry::no_checkpoints>>, sizeof...(implementations)> to_return;
            std::size_t index{0};
            ((to_return[index++] = createPosition<implementations>()), ...);
            return to_return;
        }
    private:

        template <typename implementation>
        static constexpr std::unique_ptr<Position<CancelCheckpointRegistry::no_checkpoints>> createPosition()
        {
            auto cancel_checkpoint_registry{std::make_unique<CancelCheckpointRegistry>()};
            auto context{std::make_unique<query::Context<implementation::cancelType(), implementation::cleanupType()>>(cancel_checkpoint_registry.get())};
            std::unique_ptr<tree::Node> root{tree::Builder<implementation::cancelType(), implementation::cleanupType(), delay, size>::build(context.get())};

            return std::make_unique<Position<CancelCheckpointRegistry::no_checkpoints>>(implementation::toString(), runTestOn<implementation>());
        }

        template <typename implementation>
        static constexpr std::array<std::optional<std::size_t>, CancelCheckpointRegistry::no_checkpoints> runTestOn()
        {
            CancelCheckpointRegistry cancel_checkpoint_registry;
            query::Context<implementation::cancelType(), implementation::cleanupType()> context{&cancel_checkpoint_registry};
            auto root{tree::Builder<implementation::cancelType(), implementation::cleanupType(), delay, size>::build(&context)};
            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionStarted);

            if constexpr (implementation::cleanupType() == CleanupType::kException) {
                try {
                    root->next();
                }
                catch (query::Exception&) {

                }
            }
            else {
                root->next();
            }
            cancel_checkpoint_registry.registerCheckpoint(CancelCheckpointRegistry::Checkpoint::kExecutionFinished);

            auto checkpoints{ cancel_checkpoint_registry.getCheckpoints()};
            std::array<std::optional<std::size_t>, CancelCheckpointRegistry::no_checkpoints> to_return{};

            std::transform(checkpoints.begin(), checkpoints.end(), to_return.begin(), [](const auto& tp) {
                if (tp) {
                    return std::optional<std::size_t>{tp->time_since_epoch().count()};
                }
                return std::optional<std::size_t>{std::nullopt};
            });

            return to_return;
        }
    };


}