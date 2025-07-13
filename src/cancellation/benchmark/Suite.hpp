#pragma once

#include "cancellation/benchmark/Benchmark.hpp"
#include "cancellation/benchmark/csv/Table.hpp"
#include "cancellation/benchmark/Column.hpp"
#include "cancellation/benchmark/csv/Printer.hpp"
#include "cancellation/util/Impl.hpp"
#include <fstream>


namespace cancellation::benchmark {
    class Suite {
    public:
        void runTests() {
            auto results = Benchmark<10, 1'000'000'000, util::Impl<CancelType::kAtomicEnum, CleanupType::kErrorReturn>,
            util::Impl<CancelType::kAtomicEnum, CleanupType::kException>,
            util::Impl<CancelType::kFunctionExchg, CleanupType::kException>>::run();
            csv::Table<static_cast<std::size_t>(ColumnEnum::size), ColumnType<ColumnEnum::kName>::type, ColumnType<ColumnEnum::kExecutionStarted>::type, ColumnType<ColumnEnum::kRegistered>::type, ColumnType<ColumnEnum::kCancelInitiated>::type,
            ColumnType<ColumnEnum::kExecutionFinished>::type, ColumnType<ColumnEnum::kCancelDelayMs>::type> table{std::array<std::string, static_cast<std::size_t>(ColumnEnum::size)>{ToString<ColumnEnum, ColumnEnum::kName>::value(), ToString<ColumnEnum, ColumnEnum::kExecutionStarted>::value(), ToString<ColumnEnum, ColumnEnum::kRegistered>::value(),
            ToString<ColumnEnum, ColumnEnum::kCancelInitiated>::value(), ToString<ColumnEnum, ColumnEnum::kExecutionFinished>::value(), ToString<ColumnEnum, ColumnEnum::kCancelDelayMs>::value()}};

            for (const auto& result_array : results) {
                for (const auto& result : result_array) {
                    table.pushRow(result->tuTuple());
                }
            }

            csv::Printer printer{"output.csv"};
            printer.print(table);
        }
    };
}