#pragma once

#include <array>
#include <chrono>
#include <optional>
#include <string>
#include "cancellation/benchmark/ToString.hpp"

namespace cancellation::benchmark {
    class CancelCheckpointRegistry {
    public:
        using CheckpointT = std::chrono::high_resolution_clock::time_point;

        enum class Checkpoint : std::size_t {
            kExecutionStarted = 0,
            kCancelRegistered,
            kCancelInitiated,
            kExecutionFinished
        };


        static constexpr std::size_t no_checkpoints{4};

        void registerCheckpoint(Checkpoint checkpoint);

        [[nodiscard]] std::array<std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>, no_checkpoints>
        getCheckpoints() const;

    private:
        std::array<std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>, no_checkpoints> registered_checkpoints_;
    };




    template<>
    struct ToString<CancelCheckpointRegistry::Checkpoint, CancelCheckpointRegistry::Checkpoint::kExecutionStarted> {
        static std::string value() {
            return "execution-started";
        }
    };

    template<>
    struct ToString<benchmark::CancelCheckpointRegistry::Checkpoint,
                benchmark::CancelCheckpointRegistry::Checkpoint::kCancelRegistered> {
        static std::string value() {
            return "cancel-registered";
        }
    };

    template<>
    struct ToString<benchmark::CancelCheckpointRegistry::Checkpoint,
                benchmark::CancelCheckpointRegistry::Checkpoint::kCancelInitiated> {
        static std::string value() {
            return "cancel-initiated";
        }
    };


    template<>
    struct ToString<CancelCheckpointRegistry::Checkpoint, CancelCheckpointRegistry::Checkpoint::kExecutionFinished> {
        static std::string value() {
            return "execution-finished";
        }
    };
}
