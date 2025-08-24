
#include "cancellation/time_benchmark/Suite.hpp"


int main()
{



    cancellation::time_benchmark::Suite::runTest<cancellation::util::Impl<cancellation::CancelType::kAtomicEnum, cancellation::CleanupType::kErrorReturn>>();
    //cancellation::time_benchmark::Suite::runTests<10, 10'000'000>();
}