#pragma once

#include "tree/LeafNode.hpp"
#include "CancelType.hpp"

namespace cancellation::tree
{

    template <CancelType cancel_type>
    class LeafNodeImpl : public Node
    {
    };

    template <>
    class LeafNodeImpl<CancelType::kAtomicEnum> : public Node
    {
    public:

        LeafNodeImpl(std::size_t delay_count) : delay_count_(delay_count) {}

        bool next() override
        {
            if(--delay_count_ > 0)
            {
                return true;
            }

            // we are done
            return false;
        }

    private:
        volatile std::size_t delay_count_;

    };
}