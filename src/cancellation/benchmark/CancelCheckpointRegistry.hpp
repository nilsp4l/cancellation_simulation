#pragma once

#include <chrono>
#include <optional>

namespace cancellation::benchmark
{
    class CancelCheckpointRegistry
    {
    public:
        void registerRegistered();
        void registerExecutionStarted();
        void registerExecutionFinished();

        [[nodiscard]] std::optional<double> getTillExecutionTime() const;

        [[nodiscard]] std::optional<double> getExecutionTime() const;

        [[nodiscard]] std::optional<double> getTillFinishedTime() const;

    private:

        std::optional<std::chrono::time_point<std::chrono::steady_clock>> registered_;
        std::optional<std::chrono::time_point<std::chrono::steady_clock>> execution_started_;
        std::optional<std::chrono::time_point<std::chrono::steady_clock>> execution_finished_;
    };
}