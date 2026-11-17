cmake_minimum_required(VERSION 3.20)
project(lab_04_bank LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "ENABLE_SANITIZERS requires GCC or Clang with ASan/UBSan")
    endif()
endif()

add_library(bank_lib
    src/bank_product.cpp
    src/deposit.cpp
    src/credit.cpp
    src/card.cpp
    src/product_collection.cpp
)
target_include_directories(bank_lib PUBLIC include)

add_executable(lab_04_main src/main.cpp)
target_link_libraries(lab_04_main PRIVATE bank_lib)

include(CTest)
if(BUILD_TESTING)
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
    add_executable(lab_04_tests tests/bank_test.cpp)
    target_link_libraries(lab_04_tests PRIVATE bank_lib GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(lab_04_tests)
endif()
