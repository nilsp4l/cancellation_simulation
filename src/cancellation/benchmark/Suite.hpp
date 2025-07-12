#pragma once

#include "cancellation/benchmark/Printer.hpp"
#include "cancellation/benchmark/Benchmark.hpp"
#include <fstream>


namespace cancellation::benchmark {
    class Suite {
    public:
        void runTests() {

            auto printer{Printer{"output.csv"}};
            printer.println("hello world!");
        }
    };
}