if (FILESEARCH_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED) # Assumes GTest is already installed or handled by parent CMakeLists.txt

    # Add Google Test if not found, or use FetchContent (if enabled by parent)
    if (NOT GTest_FOUND AND FILESEARCH_FETCH_GTEST)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
        )
        FetchContent_MakeAvailable(googletest)
        # Need to re-find_package after fetching
        find_package(GTest CONFIG REQUIRED)
    endif()

    # Add a test executable
    add_executable(filesearch_test test_filesearch.cpp)

    # Link against Google Test libraries and your core library
    target_link_libraries(filesearch_test PRIVATE
        GTest::gtest_main
        filesearch_core
    )

    # Include directories for the test executable
    target_include_directories(filesearch_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include # For filesearch.h
    )

    # Add the test to CTest
    add_test(NAME FilesearchUnitTest COMMAND filesearch_test)
endif()

