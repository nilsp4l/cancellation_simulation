add_library(cancel_functions SHARED CancelFunctions.cpp)

target_include_directories(cancel_functions PUBLIC ${PROJECT_SOURCE_DIR}/src)