#pragma once

#include <array>
#include <chrono>
#include <optional>
#include <string>
#include "cancellation/util/ToString.hpp"

namespace cancellation::benchmark
{
    class CancelCheckpointRegistry
    {
    public:
        using CheckpointT = std::chrono::steady_clock::time_point;
        enum class Checkpoint : std::size_t {
            kRegistered = 0,
            kExecutionStarted,
            kExecutionFinished
        };


        static constexpr std::size_t no_checkpoints{3};

        void registerCheckpoint(Checkpoint checkpoint);

        [[nodiscard]] std::array<std::optional<CheckpointT>, no_checkpoints> getCheckpoints(Checkpoint checkpoint) const;

    private:
        std::array<std::optional<CheckpointT>, no_checkpoints> registered_checkpoints_;
    };
    template <typename T, T>
    struct ToString {

    };

    template <>
    struct ToString<benchmark::CancelCheckpointRegistry::Checkpoint, benchmark::CancelCheckpointRegistry::Checkpoint::kRegistered> {
        static std::string value()
        {
            return "registered";
        }
    };

    template <>
    struct ToString<CancelCheckpointRegistry::Checkpoint, CancelCheckpointRegistry::Checkpoint::kExecutionStarted> {
        static std::string value()
        {
            return "execution started";
        }
    };

    template <>
    struct ToString<CancelCheckpointRegistry::Checkpoint, CancelCheckpointRegistry::Checkpoint::kExecutionFinished> {
        static std::string value()
        {
            return "execution finished";
        }
    };

}
