#include "../include/filesearch.h"
#include <iostream>
#include <string>
#include <vector>

void printUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <directory_to_index> <search_query>" << std::endl;
    std::cout << "Example: " << programName << " /Users/amruthavarshini/Documents my_keyword" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1; // Indicate error
    }

    fs::path directoryToIndex = argv[1];
    std::string searchQuery = argv[2];

    std::cout << "Indexing directory: " << directoryToIndex << std::endl;
    Indexer indexer;
    indexer.indexDirectory(directoryToIndex);
    std::cout << "Indexing complete." << std::endl;

    std::cout << "Searching for: '" << searchQuery << "'" << std::endl;
    Searcher searcher(indexer);
    std::vector<fs::path> results = searcher.search(searchQuery);

    if (results.empty()) {
        std::cout << "No files found containing '" << searchQuery << "'" << std::endl;
    } else {
        std::cout << "Files found:" << std::endl;
        for (const auto& path : results) {
            std::cout << "  - " << path << std::endl;
        }
    }

    return 0; // Indicate success
}

