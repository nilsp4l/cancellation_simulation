#include "different_compilation_unit/CancelFunctions.hpp"


extern "C" cancellation::util::Error notCancelledExtern() {
    return cancellation::util::Error::kSuccess;
}

extern "C" cancellation::util::Error cancelledExtern() {
    return cancellation::util::Error::kQueryCancelled;
}


extern "C" void doThrowExtern() {
    throw cancellation::query::Exception(cancellation::util::Error::kQueryCancelled);
}


extern "C" void doNotThrowExtern() {

}