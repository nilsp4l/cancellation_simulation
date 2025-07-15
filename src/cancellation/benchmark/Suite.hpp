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
        static void runTests() {
            auto results = Benchmark<10, 10'000'000, util::Impl<CancelType::kAtomicEnum, CleanupType::kErrorReturn>,
                util::Impl<CancelType::kAtomicEnum, CleanupType::kException>,
                util::Impl<CancelType::kFunctionExchg, CleanupType::kException>,
                util::Impl<CancelType::kFunctionPointerExchg, CleanupType::kException>,
                util::Impl<CancelType::kInterval, CleanupType::kErrorReturn>,
                util::Impl<CancelType::kInterval, CleanupType::kException>,
                util::Impl<CancelType::kUnion, CleanupType::kErrorReturn>,
                util::Impl<CancelType::kUnion, CleanupType::kException> >::run();
            csv::Table<static_cast<std::size_t>(ColumnEnum::size), ColumnType<ColumnEnum::kCancelType>::type, ColumnType
                <ColumnEnum::kCleanupType>::type, ColumnType<ColumnEnum::kExecutionStarted>::type, ColumnType<
                    ColumnEnum::kRegistered>::type, ColumnType<ColumnEnum::kCancelInitiated>::type,
                ColumnType<ColumnEnum::kExecutionFinished>::type, ColumnType<ColumnEnum::kCancelDelayMs>::type> table{
                std::array<std::string, static_cast<std::size_t>(ColumnEnum::size)>{
                    ToString<ColumnEnum, ColumnEnum::kCancelType>::value(),
                    ToString<ColumnEnum, ColumnEnum::kCleanupType>::value(),
                    ToString<ColumnEnum, ColumnEnum::kExecutionStarted>::value(),
                    ToString<ColumnEnum, ColumnEnum::kRegistered>::value(),
                    ToString<ColumnEnum, ColumnEnum::kCancelInitiated>::value(),
                    ToString<ColumnEnum, ColumnEnum::kExecutionFinished>::value(),
                    ToString<ColumnEnum, ColumnEnum::kCancelDelayMs>::value()
                }
            };

            for (const auto &result_array: results) {
                for (const auto &result: result_array) {
                    table.pushRow(result->tuTuple());
                }
            }

            csv::Printer printer{"output.csv"};
            printer.print(table);
        }
    };
}
