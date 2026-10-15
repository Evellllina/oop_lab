cmake_minimum_required(VERSION 3.16)
project(lab2_string LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(string_lib src/string.cpp)
if(MSVC)
    target_include_directories(string_lib PUBLIC include)
else()
    target_compile_options(string_lib PUBLIC "-iquote${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE string_lib)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()
add_executable(string_tests tests/string_test.cpp)
target_link_libraries(string_tests PRIVATE string_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(string_tests)
