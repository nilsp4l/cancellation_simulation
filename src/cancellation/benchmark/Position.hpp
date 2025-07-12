#pragma once

#include <array>
#include <string>

namespace cancellation::benchmark {

    template <std::size_t no_entries>
    class Position {

    public:


        Position(std::string name, std::array<std::optional<std::size_t>, no_entries> entries) : name_(std::move(name)), entries_(std::move(entries)) {}

        [[nodiscard]] std::string getName() const {
            return name_;
        }


        [[nodiscard]] std::array<std::optional<std::size_t>, no_entries> getEntries() const {
            return entries_;
        }


    private:
        const std::string name_;
        const std::array<std::optional<std::size_t>, no_entries> entries_;
    };
}