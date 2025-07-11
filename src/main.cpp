#include <iostream>
#include "cancellation/tree/InnerNodeImpl.hpp"
#include "cancellation/tree/LeafNodeImpl.hpp"
#include "cancellation/tree/Builder.hpp"
#include <memory>
#include <thread>

#include "cancellation/benchmark/CancelCheckpointRegistry.hpp"
#include "cancellation/CleanupType.hpp"
#include "cancellation/util/Impl.hpp"
#include "cancellation/benchmark/Benchmark.hpp"


int main()
{
    std::unique_ptr<cancellation::tree::Node> node;
    cancellation::benchmark::CancelCheckpointRegistry foo;
    auto context{std::make_unique<cancellation::query::Context<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn>>(&foo)};

    node = cancellation::tree::Builder<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn, 1'000'000'000, 10>::build(context.get());

    using namespace std::chrono_literals;
    std::thread th{[&](){std::this_thread::sleep_for(2s);
        context->markInterrupted(cancellation::util::Error::kQueryCancelled);}};
    auto ret = node->next();


    if (static_cast<cancellation::util::Error>(ret) == cancellation::util::Error::kQueryCancelled) {
        std::cout << "cancelled" << std::endl;
    }

    foo.registerCheckpoint(cancellation::benchmark::CancelCheckpointRegistry::Checkpoint::kRegistered);
   th.join();

    cancellation::benchmark::Benchmark<10, 100, cancellation::util::Impl<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn>>::run();

}