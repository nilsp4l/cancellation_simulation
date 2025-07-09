#include "benchmark/CancelCheckpointRegistry.hpp"

#include <assert.h>
#include <stdexcept>

namespace cancellation::benchmark
{
    void CancelCheckpointRegistry::registerRegistered()
    {
        registered_ = std::chrono::steady_clock::now();
    }

    void CancelCheckpointRegistry::registerExecutionStarted()
    {
        execution_started_ = std::chrono::steady_clock::now();
    }

    void CancelCheckpointRegistry::registerExecutionFinished()
    {
        execution_finished_ = std::chrono::steady_clock::now();
    }

    std::optional<double> CancelCheckpointRegistry::getTillExecutionTime() const
    {
        if (!execution_started_ || !registered_)
        {
            return std::nullopt;
        }
        assert(execution_started_ > registered_);
        return std::chrono::duration<double, std::milli>(*execution_started_ - *registered_).count();
    }

    std::optional<double> CancelCheckpointRegistry::getExecutionTime() const
    {
        if (!execution_finished_ || !execution_started_)
        {
            return std::nullopt;
        }
        assert(execution_started_ > execution_started_);
        return std::chrono::duration<double, std::milli>(*execution_finished_ - *execution_started_).count();
    }

    std::optional<double> CancelCheckpointRegistry::getTillFinishedTime() const
    {
        if (!execution_finished_ || !registered_)
        {
            return std::nullopt;
        }
        assert(execution_finished_ > execution_started_);
        return std::chrono::duration<double, std::milli>(*execution_finished_ - *registered_).count();
    }
}