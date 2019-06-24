cmake_minimum_required(VERSION 3.14)
project("Google Benchmark" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(benchmark)
find_package(Boost REQUIRED COMPONENTS headers regex)

include_directories("$ENV{SUITE_BUILD}/include")

set(LibRE2_LIBRARY "$ENV{SUITE_BUILD}/lib/libre2.a")
set(LibHYPERSCAN_LIBRARY "$ENV{SUITE_BUILD}/lib/libhs.dylib")
set(LibPCRE_LIBRARY "$ENV{SUITE_BUILD}/lib/libpcre.dylib")
set(LibPCRE2_LIBRARY "$ENV{SUITE_BUILD}/lib/libpcre2-8.dylib")

add_executable(examples examples.cpp)
add_executable(regex regex.cpp)
add_executable(regex_hyperscan regex_hyperscan.cpp)

target_compile_features(examples PUBLIC cxx_std_17)
target_compile_features(regex PUBLIC cxx_std_17)
target_compile_features(regex_hyperscan PUBLIC cxx_std_17)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(regex PRIVATE "-fvisibility=hidden")
endif()

target_link_libraries(examples PRIVATE benchmark::benchmark)
target_link_libraries(regex PRIVATE benchmark::benchmark
	                                ${Boost_LIBRARIES} Boost::regex
	                                "${LibRE2_LIBRARY}"
                                    "${LibPCRE_LIBRARY}"
	                                "${LibPCRE2_LIBRARY}")
target_link_libraries(regex_hyperscan PRIVATE benchmark::benchmark
                                              "${LibHYPERSCAN_LIBRARY}")

set_target_properties(examples PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(regex PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(regex_hyperscan PROPERTIES CXX_EXTENSIONS OFF)
