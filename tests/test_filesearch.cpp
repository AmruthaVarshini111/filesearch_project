#include <gtest/gtest.h>
#include "../include/filesearch.h" // Include your project's header
#include <fstream> // For creating dummy files

// Helper function to create a dummy file for testing
void createDummyFile(const fs::path& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "Error creating dummy file: " << filePath << std::endl;
    }
}

// Helper function to create a dummy directory structure
void createDummyDir(const fs::path& dirPath) {
    if (!fs::exists(dirPath)) {
        fs::create_directories(dirPath);
    }
}

// Test Fixture for Indexer and Searcher tests
class FileSearchTest : public ::testing::Test {
protected:
    fs::path test_dir;

    void SetUp() override {
        // Create a unique temporary directory for each test suite
        test_dir = fs::temp_directory_path() / "filesearch_test_temp";
        fs::create_directories(test_dir);

        // Create some dummy files for indexing
        createDummyDir(test_dir / "subdir1");
        createDummyDir(test_dir / "subdir2");

        createDummyFile(test_dir / "file1.txt", "Hello world this is a test.");
        createDummyFile(test_dir / "subdir1" / "file2.txt", "Another test file with content.");
        createDummyFile(test_dir / "subdir2" / "file3.cpp", "int main() { return 0; }");
        createDummyFile(test_dir / "empty.txt", ""); // Empty file
    }

    void TearDown() override {
        // Clean up the temporary directory after each test suite
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
    }
};

TEST_F(FileSearchTest, IndexerBasicFunctionality) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);

    const auto& index = indexer.getIndex();

    // Check if common words are indexed and associated with the correct files
    ASSERT_TRUE(index.count("hello"));
    ASSERT_TRUE(index.at("hello").count(test_dir / "file1.txt"));

    ASSERT_TRUE(index.count("test"));
    ASSERT_TRUE(index.at("test").count(test_dir / "file1.txt"));
    ASSERT_TRUE(index.at("test").count(test_dir / "subdir1" / "file2.txt"));

    ASSERT_FALSE(index.count("nonexistentword"));
}

TEST_F(FileSearchTest, IndexerCaseInsensitivity) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);

    const auto& index = indexer.getIndex();

    // Check for case insensitivity (e.g., "Hello" should be indexed as "hello")
    ASSERT_TRUE(index.count("hello"));
    ASSERT_TRUE(index.at("hello").count(test_dir / "file1.txt"));
}

TEST_F(FileSearchTest, SearcherBasicFunctionality) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);
    Searcher searcher(indexer);

    std::vector<fs::path> results = searcher.search("world");
    ASSERT_FALSE(results.empty());
    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0], test_dir / "file1.txt");

    results = searcher.search("test");
    ASSERT_FALSE(results.empty());
    ASSERT_EQ(results.size(), 2); // file1.txt and file2.txt contain "test"

    // Ensure order doesn't matter for sets, but for vectors we check existence
    bool found_file1 = false;
    bool found_file2 = false;
    for (const auto& p : results) {
        if (p == (test_dir / "file1.txt")) found_file1 = true;
        if (p == (test_dir / "subdir1" / "file2.txt")) found_file2 = true;
    }
    ASSERT_TRUE(found_file1);
    ASSERT_TRUE(found_file2);
}

TEST_F(FileSearchTest, SearcherNoResults) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);
    Searcher searcher(indexer);

    std::vector<fs::path> results = searcher.search("nonexistentword");
    ASSERT_TRUE(results.empty());
}

TEST_F(FileSearchTest, SearcherEmptyQuery) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);
    Searcher searcher(indexer);

    std::vector<fs::path> results = searcher.search("");
    ASSERT_TRUE(results.empty());
}

TEST_F(FileSearchTest, SearcherCaseInsensitivity) {
    Indexer indexer;
    indexer.indexDirectory(test_dir);
    Searcher searcher(indexer);

    std::vector<fs::path> results = searcher.search("HELLO"); // Search with uppercase
    ASSERT_FALSE(results.empty());
    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0], test_dir / "file1.txt");
}

// Entry point for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

