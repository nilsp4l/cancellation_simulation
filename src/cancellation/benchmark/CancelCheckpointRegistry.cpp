#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"

#include <assert.h>
#include <stdexcept>

namespace cancellation::benchmark
{
    void CancelCheckpointRegistry::registerCheckpoint(Checkpoint checkpoint) {
        auto index{static_cast<std::size_t>(checkpoint)};
        assert(index < no_checkpoints);
        registered_checkpoints_[index] = std::chrono::steady_clock::now();


    }
    std::array<std::optional<CancelCheckpointRegistry::CheckpointT>, CancelCheckpointRegistry::no_checkpoints> CancelCheckpointRegistry::getCheckpoints(Checkpoint checkpoint) const {
        auto index{static_cast<std::size_t>(checkpoint)};
        assert(index < no_checkpoints);
        return registered_checkpoints_;
    }
}