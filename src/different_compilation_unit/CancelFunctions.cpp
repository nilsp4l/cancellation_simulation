#include "different_compilation_unit/CancelFunctions.hpp"


#include "cancellation/query/Exception.hpp"

extern "C" void doThrowExtern() {
    throw cancellation::query::Exception(cancellation::util::Error::kQueryCancelled);
}


extern "C" void doNotThrowExtern() {

}