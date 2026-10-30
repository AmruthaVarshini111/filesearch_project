# Define the core library
add_library(filesearch_core STATIC
    indexer.cpp
    searcher.cpp
)

# Include directories for the core library
target_include_directories(filesearch_core PUBLIC
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Define the main executable
add_executable(filesearch main.cpp)

# Link the executable with the core library
target_link_libraries(filesearch PRIVATE filesearch_core)

