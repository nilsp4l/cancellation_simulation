#include <iostream>
#include "cancellation/tree/InnerNodeImpl.hpp"
#include "cancellation/tree/LeafNodeImpl.hpp"
#include "cancellation/tree/Builder.hpp"
#include <memory>
#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"


int main()
{
    std::unique_ptr<cancellation::tree::Node> node;

    node = cancellation::tree::Builder<CancelType::kAtomicEnum, 10'000'000, 10>::build();

    node->next();

    cancellation::benchmark::CancelCheckpointRegistry foo;
    foo.registerExecutionFinished();

}