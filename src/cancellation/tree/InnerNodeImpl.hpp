#pragma once

#include "cancellation/tree/InnerNode.hpp"
#include <array>

#include "cancellation/CleanupType.hpp"
#include "cancellation/query/Context.hpp"

namespace cancellation::tree {
    /*
    template <CancelType cancel_type, CleanupType cleanup_type>
    class InnerNodeImpl : public Node
    {
    };
    */
    template<CancelType cancel_type, CleanupType cleanup_type>
    class InnerNodeImpl : public InnerNode {
    public:
        explicit InnerNodeImpl(std::array<std::unique_ptr<Node>, InnerNode::no_children> inputs,
                               query::Context<cancel_type, cleanup_type> *context) : inputs_(std::move(inputs)),
            context_(context) {
        }

        int next() override {
            std::for_each(inputs_.rbegin(), inputs_.rend(), [this](std::unique_ptr<Node> &node) {
                while (!static_cast<int>(node->next()));
            });


            return static_cast<int>(true);
        }

    private:
        std::array<std::unique_ptr<Node>, InnerNode::no_children> inputs_;
        query::Context<cancel_type, cleanup_type> *context_;
    };
}
