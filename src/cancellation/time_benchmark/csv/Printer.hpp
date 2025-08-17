#pragma once
#include <fstream>
#include <string>
#include <optional>
#include <iostream>
#include "cancellation/time_benchmark/csv/Table.hpp"

namespace cancellation::time_benchmark::csv {

    static std::string toString(std::size_t i) {
        return std::to_string(i);
    }

    static std::string toString(std::string text) {
        return text;
    }

    template<typename T>
    static std::string toString(std::optional<T> opt) {
        if (!opt) {
            return "NA";
        }

        return toString(*opt);
    }



    template<bool print_to_console = true>
    class Printer {
    public:
        explicit Printer(const std::string& file_name) : stream_(std::ofstream(file_name, std::ios::trunc)) {
        }

        template<std::size_t no_columns, typename... row_types>
        void print(Table<no_columns, row_types...> table) {
            if constexpr (print_to_console) {
                std::cout << toCsv(table.getColumnNames()) << std::endl;
            }
            stream_ << toCsv(table.getColumnNames()) << std::endl;

            for (const auto& tuple : table) {
                auto line{toCsv(tuple)};
                if constexpr(print_to_console) {
                    std::cout << line << std::endl;
                }
                stream_ << line << std::endl;
            }
        }

    private:
        template<std::size_t array_size>
        static std::string toCsv(std::array<std::string, array_size> array) {
            std::string to_return;
            for (std::size_t i = 0; i < array_size; ++i) {
                to_return += array[i] + (i == array_size - 1 ? "" : ",");
            }
            return to_return;
        }

        template<typename... column_types>
        static std::string toCsv(const std::tuple<column_types...>& tuple) {
            std::string to_return;
            std::size_t no_columns = sizeof...(column_types);
            std::size_t index{0};
            std::apply([&to_return, no_columns, &index](auto const&... arg) {
                ((to_return += toString(arg) + (index++ == no_columns - 1 ? "" : ",")), ...);
            }, tuple);

            return to_return;
        }

        std::ofstream stream_;
    };
}
