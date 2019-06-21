cmake_minimum_required(VERSION 3.14)
project("Google Benchmark" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(benchmark)

include_directories("$ENV{SUITE_BUILD}/include")

add_executable(examples examples.cpp)

target_compile_features(examples PUBLIC cxx_std_17)

target_link_libraries(examples PRIVATE benchmark::benchmark)

set_target_properties(examples PROPERTIES CXX_EXTENSIONS OFF)
