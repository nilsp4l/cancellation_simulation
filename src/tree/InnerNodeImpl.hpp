#pragma once

#include "tree/InnerNode.hpp"
#include <array>

namespace cancellation::tree
{
    template <CancelType cancel_type>
    class InnerNodeImpl : public Node
    {
    };

    template <>
    class InnerNodeImpl<CancelType::kAtomicEnum> : public InnerNode
    {
    public:
        InnerNodeImpl(std::array<std::unique_ptr<Node>, InnerNode::no_children> inputs) : inputs_(std::move(inputs)) {}

        bool next() override
        {
            for(auto& input : inputs_)
            {
                while (input->next())
                ;
            }

            return false;
        }

    private:
        std::array<std::unique_ptr<Node>, InnerNode::no_children> inputs_;
    };

}