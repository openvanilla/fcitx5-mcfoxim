#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/completer.h"
#include "../src/inputtable.h"

using namespace McFoxIM;

void createTestFile(const std::string& filename) {
  std::ofstream out(filename);
  out << R"({
        "name": "TestTable",
        "data": [
            ["a", "A"],
            ["ab", "AB"],
            ["abc", "ABC"],
            ["b", "B"],
            ["ba", "BA"]
        ]
    })";
  out.close();
}

void testCompleter() {
  std::string testFile = "test_data.json";
  createTestFile(testFile);

  InputTable table;
  bool loaded = table.load(testFile);
  assert(loaded && "Failed to load test data");

  Completer completer([&table]() -> const InputTable& { return table; });

  // Test exact match
  auto results = completer.complete("a");
  assert(results.size() >= 3);
  assert(results[0].displayText() == "a");
  assert(results[0].description() == "A");
  assert(results[1].displayText() == "ab");
  assert(results[1].description() == "AB");
  assert(results[2].displayText() == "abc");
  assert(results[2].description() == "ABC");

  // Test prefix match
  results = completer.complete("ab");
  assert(results.size() >= 2);
  assert(results[0].displayText() == "ab");
  assert(results[0].description() == "AB");
  assert(results[1].displayText() == "abc");
  assert(results[1].description() == "ABC");

  // Test no match
  results = completer.complete("z");
  assert(results.empty());

  // Test partial match
  results = completer.complete("b");
  assert(results.size() >= 2);
  assert(results[0].displayText() == "b");
  assert(results[0].description() == "B");
  assert(results[1].displayText() == "ba");
  assert(results[1].description() == "BA");

  std::filesystem::remove(testFile);
  std::cout << "All tests passed!" << std::endl;
}

int main() {
  testCompleter();
  return 0;
}
