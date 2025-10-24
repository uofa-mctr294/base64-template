cmake_minimum_required(VERSION 3.14)

# Set the project name and version
project(HelloWorld VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executables
add_executable(hello_world src/helloWorld/main.cpp)
add_executable(greet_world src/greet/main.cpp)

# Small library providing the greeting function so unit tests can link to it.
add_library(greet src/greet/greet.h src/greet/greet.cpp)
target_include_directories(greet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/greet)
target_compile_features(greet PUBLIC cxx_std_17)
target_link_libraries(greet_world PRIVATE greet)

# --- GoogleTest (for integration tests that run the built binary) ---
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test that runs the built binary. The test source will launch the binary by path.
add_executable(test_binary tests/test_binary.cpp)
target_link_libraries(test_binary PRIVATE GTest::gtest)

# Unit test that contains a copy of the program's main logic (keeps the installed
# `hello_world` executable unchanged). The test captures stdout from the copied
# main logic and asserts the expected output.
add_executable(test_unit tests/test_unit.cpp)
target_link_libraries(test_unit PRIVATE greet GTest::gtest_main)
add_test(NAME unit_main_test COMMAND test_unit)

# Provide the path to the built executable to the test via a compile definition.
if (CMAKE_GENERATOR MATCHES "Visual Studio")
	# Visual Studio places executables in configuration subfolders (Debug/Release)
	set(EXE_SUBDIR $<CONFIG>)
else()
	set(EXE_SUBDIR "")
endif()

add_test(NAME run_hello_binary COMMAND test_binary $<TARGET_FILE:hello_world>)
