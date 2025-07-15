#pragma once
#include <string>
#include <variant>
#include "cancellation/util/Visitor.hpp"

namespace cancellation::util {
    class Status {
    public:
        class Error {
        public:
            explicit Error(std::string error_message) : error_message_(std::move(error_message)) {
            }

            std::string getErrorMessage() { return error_message_; }

        private:
            std::string error_message_;
        };

        class Ok {
        };



        Status &operator=(const Error &error) {
            status_ = error;
            return *this;
        }



        Status &operator=(const Error &&error) {
            status_ = error;
            return *this;
        }



        Status &operator=(const Ok &ok) {
            status_ = ok;
            return *this;
        }



        Status &operator=(const Ok &&ok) {
            status_ = ok;
            return *this;
        }



        explicit operator bool() const {
            return std::visit(Visitor{
                                  [](const Ok &) {
                                      return true;
                                  },
                                  [](const Error &) {
                                      return false;
                                  }
                              }, status_);
        }

    private:
        std::variant<Ok, Error> status_ {Ok{}};
    };
}
