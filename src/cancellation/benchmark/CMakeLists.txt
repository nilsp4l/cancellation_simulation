add_library(benchmark STATIC CancelCheckpointRegistry.cpp)


target_include_directories(benchmark PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(csv)

set_target_properties(benchmark PROPERTIES COMPILE_FLAGS "-save-temps=obj")