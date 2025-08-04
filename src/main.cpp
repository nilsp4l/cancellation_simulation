
#include "cancellation/benchmark/Suite.hpp"


int main()
{


    cancellation::benchmark::Suite::runTest<cancellation::util::Impl<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn>>();
    //cancellation::benchmark::Suite::runTests();
}