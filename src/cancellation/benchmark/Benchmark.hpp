#pragma once

#include "cancellation/tree/Node.hpp"
#include "cancellation/tree/Builder.hpp"
#include "cancellation/query/Context.hpp"
#include <algorithm>
#include "cancellation/benchmark/Test.hpp"

namespace cancellation::benchmark {
    template<std::size_t size, std::size_t delay, typename... implementations>
    struct Benchmark {
    public:

        static constexpr std::size_t no_cancel_delays{4};
        static constexpr std::array<std::size_t, no_cancel_delays> cancel_delays{{10, 100, 2000, 4000}};
        static constexpr std::size_t no_repetitions{5};

        static constexpr std::size_t no_tests{no_cancel_delays * no_repetitions};

        static constexpr std::array<std::array<std::unique_ptr<Result>, no_tests>, sizeof...(implementations)> run() {
            std::array<std::array<std::unique_ptr<Result>, no_tests>, sizeof...(implementations)> to_return;
            std::size_t index{0};
            ((to_return[index++] = runTestsOn<implementations>()), ...);
            return to_return;
        }

    private:
        template<typename implementation>
        static constexpr std::array<std::unique_ptr<Result>, no_tests> runTestsOn() {
            std::array<std::unique_ptr<Result>, no_tests> tests;
            std::size_t index{0};
            for (auto cancel_delay: cancel_delays) {
                for (std::size_t i{0}; i < no_repetitions; ++i) {
                    tests[index++] = std::make_unique<Result>(Test<implementation, size, delay>::run(cancel_delay));
                }

            }

            return tests;
        }
    };
}
