#include "../include/filesearch.h"
#include <algorithm> // For std::set_intersection

Searcher::Searcher(const Indexer& indexer) : indexer(indexer) {}

std::vector<fs::path> Searcher::search(const std::string& query) const {
    std::vector<fs::path> results;
    if (query.empty()) {
        return results;
    }

    // Convert query to lowercase for case-insensitive search
    std::string lower_query = query;
    std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    const auto& index = indexer.getIndex();

    // Check if the query word exists in the index
    auto it = index.find(lower_query);
    if (it != index.end()) {
        // Convert the set of paths to a vector
        for (const auto& path : it->second) {
            results.push_back(path);
        }
    }

    return results;
}

