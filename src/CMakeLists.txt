set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME main)
include(FetchContent)

set(BENCHMARK_ENABLE_LIBPFM ON)

add_executable(${EXECUTABLE_NAME} main.cpp)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(cancellation)

add_subdirectory(different_compilation_unit)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG c58e6d0710581e3a08d65c349664128a8d9a2461 #v1.9.1
)

FetchContent_MakeAvailable(googletest googlebenchmark)


target_link_libraries(${EXECUTABLE_NAME} PUBLIC time_benchmark)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC util)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE cancel_functions)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "-save-temps=obj")
target_link_libraries(main PRIVATE atomic)