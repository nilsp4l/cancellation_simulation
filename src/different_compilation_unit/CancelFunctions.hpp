#pragma once
#include "cancellation/query/Exception.hpp"

extern "C" cancellation::util::Error notCancelledExtern();

extern "C" cancellation::util::Error cancelledExtern();

extern "C" void doThrowExtern();

extern "C" void doNotThrowExtern();
