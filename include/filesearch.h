#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <filesystem> // C++17 filesystem

namespace fs = std::filesystem;

class Indexer {
public:
    void indexDirectory(const fs::path& directoryPath);

    // Declaration and definition of the getter for the index.
    // It's placed here so Searcher can access it.
    const std::map<std::string, std::set<fs::path>>& getIndex() const {
        return index;
    }

private:
    std::map<std::string, std::set<fs::path>> index; // word -> {file paths}
    // Declarations for member functions implemented in indexer.cpp
    // These are private as they are called internally.
    void indexFile(const fs::path& filePath);
    std::set<std::string> tokenize(const std::string& text);
};

class Searcher {
public:
    explicit Searcher(const Indexer& indexer);
    std::vector<fs::path> search(const std::string& query) const;
private:
    const Indexer& indexer;
};

#endif // FILESEARCH_H

