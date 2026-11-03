cmake_minimum_required(VERSION 3.20)
project(lab_03_dateset LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(dateset_lib src/date.cpp src/date_set.cpp)
target_include_directories(dateset_lib PUBLIC include)

add_executable(lab_03_main src/main.cpp)
target_link_libraries(lab_03_main PRIVATE dateset_lib)

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
add_executable(lab_03_tests tests/date_test.cpp tests/date_set_test.cpp)
target_link_libraries(lab_03_tests PRIVATE dateset_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab_03_tests)
