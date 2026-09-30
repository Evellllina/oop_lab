cmake_minimum_required(VERSION 3.16)
project(lab01_variant5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main_app 
    src/main.cpp 
    src/array_ops.cpp 
    src/variant_5.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests 
    tests/test.cpp 
    src/array_ops.cpp 
    src/variant_5.cpp
)

target_link_libraries(unit_tests PRIVATE gtest_main)
target_include_directories(unit_tests PRIVATE src)
target_include_directories(main_app PRIVATE src)

include(GoogleTest)
gtest_discover_tests(unit_tests)