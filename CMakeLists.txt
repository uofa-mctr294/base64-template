cmake_minimum_required(VERSION 3.14)

project(base64 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Always enable ImageInfo parser; include headers from extern/imageinfo
set(IMAGEINFO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/imageinfo/include" CACHE PATH "Path to imageinfo include directory" FORCE)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Library from all src/*.cpp except main.cpp
add_library(base64_lib ${SRC_FILES})
target_include_directories(base64_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(base64_lib PUBLIC cxx_std_17)

# Add ImageInfo include path unconditionally
if (IMAGEINFO_INCLUDE_DIR)
    target_include_directories(base64_lib PRIVATE ${IMAGEINFO_INCLUDE_DIR})
endif()

# Executable only contains main and links to the library
add_executable(base64 ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(base64 PRIVATE base64_lib)
target_include_directories(base64 PRIVATE ${IMAGEINFO_INCLUDE_DIR})
target_compile_features(base64 PRIVATE cxx_std_17)

target_compile_features(base64 PRIVATE cxx_std_17)

# --- GoogleTest setup and test wiring ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_binary tests/test_binary.cpp)
target_link_libraries(test_binary PRIVATE GTest::gtest)

# Pass executable path, input base64 path, and output html path to the test
add_test(
    NAME image_size_test
    COMMAND test_binary
        $<TARGET_FILE:base64>
        ${CMAKE_SOURCE_DIR}/images/minions/happy-minions.b64
        $<TARGET_FILE_DIR:base64>/test_output.html
)

# Unit tests for base64 library
add_executable(test_unit tests/test_unit.cpp)
target_link_libraries(test_unit PRIVATE base64_lib GTest::gtest_main)
## Include dirs provided by base64_lib PUBLIC, no need to add here
add_test(NAME unit_base64_tests COMMAND test_unit)
