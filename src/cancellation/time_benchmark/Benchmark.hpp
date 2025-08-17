#pragma once


#include "cancellation/tree/Builder.hpp"

#include "cancellation/time_benchmark/Test.hpp"

namespace cancellation::time_benchmark {
    template<std::size_t size, std::size_t delay, typename... implementations>
    struct Benchmark {
    public:

        static constexpr std::size_t no_cancel_delays{5};
        static constexpr std::array<std::optional<std::size_t>, no_cancel_delays> cancel_delays{{std::nullopt, 10, 105, 2121, 4321}};
        static constexpr std::size_t no_repetitions{20};

        static constexpr std::size_t no_tests{no_cancel_delays * no_repetitions};

        static constexpr std::array<std::array<std::unique_ptr<Result>, no_tests>, sizeof...(implementations)> run() {
            std::array<std::array<std::unique_ptr<Result>, no_tests>, sizeof...(implementations)> to_return;
            std::size_t index{0};
            ((to_return[index++] = runTestsOn<implementations>()), ...);
            return to_return;
        }

    private:
        template<typename implementation>
        static inline std::array<std::unique_ptr<Result>, no_tests> runTestsOn() {
            std::array<std::unique_ptr<Result>, no_tests> tests;
            std::size_t index{0};
            for (auto cancel_delay: cancel_delays) {
                for (std::size_t i{0}; i < no_repetitions; ++i) {
                    tests[index++] = std::make_unique<Result>(Test<implementation, size, delay>::run(cancel_delay));
                }
            }

            std::cout << implementation::toString() << " test done" << std::endl;

            return tests;
        }
    };
}
