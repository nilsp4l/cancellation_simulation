#pragma once

#include <stdexcept>
#include "cancellation/util/ExecutionReturnValue.hpp"


namespace cancellation::query {
    class Exception : public std::exception {
    public:
        explicit Exception(util::Error error) : error_(error) {
        }

        util::Error error() const { return error_; }

    private:
        util::Error error_;
    };
}
