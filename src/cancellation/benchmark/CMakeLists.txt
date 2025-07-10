add_library(benchmark STATIC CancelCheckpointRegistry.cpp)

target_include_directories(benchmark PUBLIC ${PROJECT_SOURCE_DIR}/src)