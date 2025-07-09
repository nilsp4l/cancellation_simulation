#pragma once

#include "tree/LeafNodeImpl.hpp"
#include "tree/InnerNodeImpl.hpp"

namespace cancellation::tree
{
    template <CancelType cancel_type, std::size_t delay_amount, std::size_t size>
    struct Builder
    {
        static constexpr std::unique_ptr<Node> build()
        {
            return std::make_unique<InnerNodeImpl<cancel_type>>(std::array<std::unique_ptr<Node>, InnerNode::no_children>{std::make_unique<LeafNodeImpl<cancel_type>>(delay_amount), Builder<cancel_type, delay_amount, size - 1>::build()});
        }
    };

    template <CancelType cancel_type, std::size_t delay_amount>
    struct Builder<cancel_type, delay_amount, 0>
    {
        static constexpr std::unique_ptr<Node> build()
        {
            return std::make_unique<LeafNodeImpl<cancel_type>>(delay_amount);
        }
    };

}