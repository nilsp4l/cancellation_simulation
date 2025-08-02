
#include "cancellation/benchmark/Suite.hpp"


int main()
{


    //cancellation::benchmark::Suite::runTest<cancellation::util::Impl<cancellation::CancelType::kUnion, cancellation::CleanupType::kException>>();
    cancellation::benchmark::Suite::runTests();
}