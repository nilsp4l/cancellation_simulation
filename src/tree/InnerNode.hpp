#pragma once

#include "tree/Node.hpp"
#include "CancelType.hpp"
#include <iostream>
#include <memory>

namespace cancellation::tree
{
    // can be understood as a simulation of a scan node
    class InnerNode : public Node
    {
        public:
        static constexpr std::size_t no_children{2};
    };

}