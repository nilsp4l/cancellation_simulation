
#include "cancellation/time_benchmark/Suite.hpp"


int main()
{



    //cancellation::benchmark::Suite::runTest<cancellation::util::Impl<cancellation::CancelType::kFunctionPointerExchgDifferentCompUnit, cancellation::CleanupType::kException>>();
    cancellation::time_benchmark::Suite::runTests<10, 10'000'000>();
}