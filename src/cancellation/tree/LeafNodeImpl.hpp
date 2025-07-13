#pragma once

#include "cancellation/tree/LeafNode.hpp"
#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/query/Context.hpp"

namespace cancellation::tree {
    /*
    template<CancelType, CleanupType>
    class LeafNodeImpl : public Node {
    };
    */

    template<CancelType cancel_type, CleanupType cleanup_type>
    class LeafNodeImpl: public Node {
    public:
        explicit LeafNodeImpl(std::size_t delay_count,
                              query::Context<cancel_type, cleanup_type> *
                              context) : delay_count_(delay_count), context_(context) {
        }



        int next() override {
            if constexpr (cleanup_type == CleanupType::kErrorReturn) {
                if (auto error = context_->checkForInterrupt(); static_cast<bool>(error)) {
                    return static_cast<int>(error);
                }
            }
            else {
                context_->checkForInterrupt(); // throws
            }

            if (--delay_count_ > 0) {
                return static_cast<int>(false);
            }

            // we are done
            return static_cast<int>(true);
        }


    private:
        volatile std::size_t delay_count_;
        query::Context<cancel_type, cleanup_type> *context_;
    };
}
