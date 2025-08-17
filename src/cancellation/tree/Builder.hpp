#pragma once

#include "cancellation/tree/LeafNodeImpl.hpp"
#include "cancellation/tree/InnerNodeImpl.hpp"
#include "cancellation/query/Context.hpp"

namespace cancellation::tree {
    template<CancelType cancel_type, CleanupType cleanup_type, std::size_t delay_amount, std::size_t size>
    struct Builder {
        static inline std::unique_ptr<Node> build(query::Context<cancel_type, cleanup_type> *context, time_benchmark::CancelCheckpointRegistry* cancel_checkpoint_registry) {
            return std::make_unique<InnerNodeImpl<cancel_type, cleanup_type> >(
                std::array<std::unique_ptr<Node>, InnerNode::no_children>{
                    Builder<cancel_type, cleanup_type, delay_amount, size - 1>::build(context, cancel_checkpoint_registry),
                    std::make_unique<LeafNodeImpl<cancel_type, cleanup_type> >(delay_amount, context, cancel_checkpoint_registry)
                }, context);
        }
    };

    template<CancelType cancel_type, CleanupType cleanup_type, std::size_t delay_amount>
    struct Builder<cancel_type, cleanup_type, delay_amount, 0> {
        static inline std::unique_ptr<Node> build(query::Context<cancel_type, cleanup_type> *context, time_benchmark::CancelCheckpointRegistry* cancel_checkpoint_registry) {
            return std::make_unique<LeafNodeImpl<cancel_type, cleanup_type> >(delay_amount, context, cancel_checkpoint_registry);
        }
    };
}
