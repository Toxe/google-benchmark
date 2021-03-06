cmake_minimum_required(VERSION 3.14)
project("Google Benchmark" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(benchmark CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)
find_package(re2 CONFIG REQUIRED)

find_library(Hyperscan_LIBRARY hs)
find_library(PCRE_LIBRARY pcre)
find_library(PCRE2_LIBRARY pcre2-8)

find_path(Hyperscan_INCLUDE_DIRS hs/hs.h)
find_path(PCRE_INCLUDE_DIRS pcre.h)
find_path(PCRE2_INCLUDE_DIRS pcre2.h)

set(ALL_TARGETS
    examples
    regex
    regex_hyperscan
    project_euler
    mandelbrot
    maze_generator
    string_concat
    string_split
    read_performance_test_data
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    target_compile_features(${target} PUBLIC cxx_std_20)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    target_link_libraries(${target} PRIVATE benchmark::benchmark)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wmost -pedantic -Wconversion -Wfloat-equal -Wold-style-cast -fcolor-diagnostics)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif()
endforeach()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(examples PRIVATE -Wno-sign-conversion)
    target_compile_options(regex PRIVATE -Wno-conversion -Wno-old-style-cast)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(examples PRIVATE /wd4244)
    target_compile_options(regex PRIVATE /wd4244)
    target_compile_options(regex_hyperscan PRIVATE /wd4244)
endif()

target_include_directories(regex_hyperscan PRIVATE ${Hyperscan_INCLUDE_DIRS})

target_link_libraries(regex PRIVATE ${Boost_LIBRARIES} re2::re2 "${PCRE_LIBRARY}" "${PCRE2_LIBRARY}")
target_link_libraries(regex_hyperscan PRIVATE "${Hyperscan_LIBRARY}")
