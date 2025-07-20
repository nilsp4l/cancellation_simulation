set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME main)


add_executable(${EXECUTABLE_NAME} main.cpp)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(cancellation)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC benchmark)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC util)