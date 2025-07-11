#pragma once

#include "cancellation/benchmark/Position.hpp"
#include "cancellation/tree/Node.hpp"
#include "cancellation/tree/Builder.hpp"

namespace cancellation::benchmark {
    template <std::size_t size, std::size_t delay, typename... implementations>
    struct Benchmark {
    public:
        static constexpr std::array<std::unique_ptr<Position>, sizeof...(implementations)>  run() {
            std::array<std::unique_ptr<Position>, sizeof...(implementations)> to_return;
            std::size_t index{0};
            ((to_return[index++] = createPosition<implementations>()), ...);
            return to_return;
        }
    private:
        template<CancelCheckpointRegistry::Checkpoint begin, CancelCheckpointRegistry::Checkpoint end>
        struct Delta {
            static constexpr std::chrono::duration<std::size_t, std::nano> value(const std::array<CancelCheckpointRegistry::CheckpointT, CancelCheckpointRegistry::no_checkpoints>& checkpoints) {
                return std::chrono::duration<std::size_t, std::nano>(
                    checkpoints[static_cast<std::size_t>(end)] - checkpoints[static_cast<std::size_t>(begin)]);
            }
        };


        template <typename implementation>
        static constexpr std::unique_ptr<Position> createPosition()
        {
            auto cancel_checkpoint_registry{std::make_unique<CancelCheckpointRegistry>()};
            auto context{std::make_unique<query::Context<implementation::cancelType(), implementation::cleanupType()>>(cancel_checkpoint_registry.get())};
            std::unique_ptr<tree::Node> root{tree::Builder<implementation::cancelType(), implementation::cleanupType(), delay, size>::build(context.get())};

            return std::make_unique<Position>(implementation::toString(), std::array<std::size_t, Position::no_deltas>{});
        }
    };


}