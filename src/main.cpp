
#include "cancellation/benchmark/Suite.hpp"


int main()
{



    cancellation::benchmark::Suite::runTest<cancellation::util::Impl<cancellation::CancelType::kFunctionPointerExchgDifferentCompUnit, cancellation::CleanupType::kException>>();
    //cancellation::benchmark::Suite::runTests<0, 1'000'000>();
}