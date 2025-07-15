#pragma once

#include <stdexcept>
#include "cancellation/util/ExecutionReturnValue.hpp"


namespace cancellation::query {
    template <typename T>
    class Exception : public std::exception {
    public:
        explicit Exception(T error) : error_(error) {
        }

        T error() const { return error_; }

    private:
        T error_;
    };

    template <typename T>
    Exception(T) -> Exception<T>;
}
