#pragma once

namespace cancellation::tree
{
    class Node
    {
        public:
        virtual ~Node() = default;

        virtual int next() = 0;
    };

}