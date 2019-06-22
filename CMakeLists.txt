cmake_minimum_required(VERSION 3.14)
project("Google Benchmark" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(benchmark)
find_package(Boost REQUIRED COMPONENTS headers regex)

include_directories("$ENV{SUITE_BUILD}/include")

add_executable(examples examples.cpp)
add_executable(regex regex.cpp)

target_compile_features(examples PUBLIC cxx_std_17)
target_compile_features(regex PUBLIC cxx_std_17)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(regex PRIVATE "-fvisibility=hidden")
endif()

target_link_libraries(examples PRIVATE benchmark::benchmark)
target_link_libraries(regex PRIVATE benchmark::benchmark
	                                ${Boost_LIBRARIES} Boost::regex)

set_target_properties(examples PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(regex PROPERTIES CXX_EXTENSIONS OFF)
