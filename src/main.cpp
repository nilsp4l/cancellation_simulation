#include <iostream>
#include "cancellation/tree/InnerNodeImpl.hpp"
#include "cancellation/tree/LeafNodeImpl.hpp"
#include "cancellation/tree/Builder.hpp"
#include <memory>
#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"
#include "cancellation/CleanupType.hpp"


int main()
{
    std::unique_ptr<cancellation::tree::Node> node;
    auto context{std::make_unique<cancellation::query::Context<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn>>(nullptr)};

    node = cancellation::tree::Builder<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn, 10'000'000, 10>::build(context.get());

    node->next();

    cancellation::benchmark::CancelCheckpointRegistry foo;
    foo.registerExecutionFinished();

}