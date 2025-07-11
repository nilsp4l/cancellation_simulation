#pragma once

#include <array>
#include <string>

class Position {

public:
    static constexpr std::size_t no_deltas{3};
    Position(std::string name, std::array<std::size_t, no_deltas> entries) : name_(std::move(name)), entries_(entries) {}

    [[nodiscard]] std::string getName() const {
        return name_;
    }

    [[nodiscard]] std::array<std::size_t, no_deltas> getEntries() const
    {
        return entries_;
    }
private:
    const std::string name_;
    const std::array<std::size_t, no_deltas> entries_;
};