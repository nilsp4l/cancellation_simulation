#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
namespace cancellation::benchmark::csv {



    template <std::size_t no_columns, typename... row_types>
    class Table {
    public:
        using RowT = std::tuple<row_types...>;

        Table(const std::array<std::string, no_columns>& column_names) : column_names_(column_names) {
            static_assert(no_columns == sizeof...(row_types));

        }

        std::array<std::string, no_columns> getColumnNames() const {

            return column_names_;
        }

        void pushRow(RowT row) {
            rows_.push_back(std::move(row));
        }


        typename std::vector<RowT>::const_iterator cbegin() const {
            return rows_.cbegin();
        }

        typename std::vector<RowT>::const_iterator cend() const {
            return rows_.cend();
        }

        typename std::vector<RowT>::const_iterator begin() const {
            return rows_.begin();
        }

        typename std::vector<RowT>::const_iterator end() const {
            return rows_.end();
        }

    private:
        std::array<std::string, no_columns> column_names_;
        std::vector<RowT> rows_;
    };
}
