cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(filesearch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable recommended compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Special handling for older GCC versions for std::filesystem
# This is often needed on systems with GCC < 9
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    message(STATUS "GCC version less than 9.0 detected. Adding -lstdc++fs for std::filesystem.")
    add_compile_options(-fexperimental-library)
    link_libraries(stdc++fs)
endif()

# Option to build tests (default to ON)
option(FILESEARCH_BUILD_TESTS "Build Google Test-based tests" ON)

# Option to fetch GoogleTest (default to OFF, assuming system-wide install or manual management for now)
option(FILESEARCH_FETCH_GTEST "Fetch Google Test as a submodule or via FetchContent" OFF)

# Add subdirectories for the core library, executable, and tests
add_subdirectory(src)

if (FILESEARCH_BUILD_TESTS)
    enable_testing() # Enable CTest
    add_subdirectory(tests)
endif()

