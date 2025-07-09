#include <iostream>
#include "tree/InnerNodeImpl.hpp"
#include "tree/LeafNodeImpl.hpp"
#include "tree/Builder.hpp"
#include <memory>


int main()
{
    std::unique_ptr<cancellation::tree::Node> node;

    node = cancellation::tree::Builder<CancelType::kAtomicEnum, 1'000'000'000, 10>::build();

    node->next();

}