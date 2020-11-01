cmake_minimum_required(VERSION 3.1.3)

set(CMAKE_CXX_STANDARD 17)

project(canlib C CXX)


set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage --coverage")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov --coverage")
set(GCC_DEBUG_FLAGS "-g -ggdb -Og")
set(GCC_WARNING_FLAGS "-Wall -Wextra -fdiagnostics-color=always -Werror")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ${GCC_WARNING_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(../src)
include_directories(./tests/inc)
include_directories(../src/drivers/inc)
include_directories(../src/computers/inc)

file(GLOB CANLIB_SOURCES "../src/*.cpp" "../src/*.c" "../src/drivers/src/*.cpp" "../src/drivers/src/*.c" "../src/computers/src/canlib_testcomp0.cpp")
add_library(canlib_src SHARED ${CANLIB_SOURCES})
target_compile_features(canlib_src INTERFACE cxx_strong_enums)
target_compile_definitions(canlib_src PUBLIC CANLIB_ARCH_TESTFAMILY)

file(GLOB_RECURSE TEST_SOURCES "./tests/src/*.cpp")
add_library(canlib_tests SHARED ${TEST_SOURCES})
target_link_libraries(canlib_tests PUBLIC canlib_src)

add_executable(${PROJECT_NAME} "./tests/main.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC canlib_src)
target_link_libraries(${PROJECT_NAME} PUBLIC canlib_tests)

enable_testing()
add_test(Tests ${PROJECT_NAME})
