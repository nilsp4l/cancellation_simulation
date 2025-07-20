add_library(util STATIC Clock.cpp)


target_include_directories(util PUBLIC ${PROJECT_SOURCE_DIR}/src)