add_library(time_benchmark STATIC CancelCheckpointRegistry.cpp)


target_include_directories(time_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(csv)
add_subdirectory(google_benchmark)

set_target_properties(time_benchmark PROPERTIES COMPILE_FLAGS "-save-temps=obj")