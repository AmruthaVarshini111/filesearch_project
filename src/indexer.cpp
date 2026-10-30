#include "../include/filesearch.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower and std::ispunct

// Helper function to convert string to lowercase
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

void Indexer::indexDirectory(const fs::path& directoryPath) {
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "Error: Directory does not exist or is not a directory: " << directoryPath << std::endl;
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry.path())) {
            indexFile(entry.path());
        }
    }
}

void Indexer::indexFile(const fs::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::set<std::string> tokens = tokenize(line);
        for (const std::string& token : tokens) {
            if (!token.empty()) {
                index[token].insert(filePath);
            }
        }
    }
}

std::set<std::string> Indexer::tokenize(const std::string& text) {
    std::set<std::string> tokens;
    std::string cleaned_text = text;

    // Remove punctuation and convert to lowercase
    std::transform(cleaned_text.begin(), cleaned_text.end(), cleaned_text.begin(),
        [](unsigned char c){
            if (std::ispunct(c)) return ' '; // Replace punctuation with space
            return static_cast<char>(std::tolower(c));
        });

    std::stringstream ss(cleaned_text);
    std::string word;
    while (ss >> word) {
        if (!word.empty()) {
            tokens.insert(word);
        }
    }
    return tokens;
}

