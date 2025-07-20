cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Release)

project(approach_compare)


add_compile_options(-Wall -Wextra -Wconversion -pedantic -Wfatal-errors -g -fsanitize=leak -ftemplate-depth=2000 -O3)


add_subdirectory(src)