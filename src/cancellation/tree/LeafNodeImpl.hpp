#pragma once

#include "cancellation/tree/LeafNode.hpp"
#include "cancellation/CancelType.hpp"
#include "cancellation/CleanupType.hpp"

namespace cancellation::tree {
    template<CancelType cancel_type, CleanupType cleanup_type>
    class LeafNodeImpl : public Node {
    };

    template<>
    class LeafNodeImpl<CancelType::kAtomicEnum, CleanupType::kErrorReturn> : public Node {
    public:
        explicit LeafNodeImpl(std::size_t delay_count,
                              query::Context<CancelType::kAtomicEnum, CleanupType::kErrorReturn> *
                              context) : delay_count_(delay_count), context_(context) {
        }

        int next() override {
            if (--delay_count_ > 0) {
                return static_cast<int>(false);
            }

            // we are done
            return static_cast<int>(true);
        }

    private:
        volatile std::size_t delay_count_;
        query::Context<CancelType::kAtomicEnum, CleanupType::kErrorReturn> *context_;
    };
}
