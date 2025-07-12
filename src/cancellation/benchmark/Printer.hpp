#pragma once
#include <fstream>
#include <string>

namespace cancellation::benchmark {
    template <bool print_to_console = true>
    class Printer {
    public:

        explicit Printer(std::string file_name) : stream(std::ofstream(file_name, std::ios::trunc)) {

        }

        void println(std::string text) {

            if constexpr (print_to_console) {
                std::cout << text << std::endl;
            }
            stream << text << std::endl;

        }

    private:
        std::ofstream stream;
    };
}