#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

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
            ["ba", "BA"],
            ["dup", "D1"],
            ["dup", "D2"],
            ["long", "LONG"],
            ["longer", "LONGER"]
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

  // Test exact match and sorting by length
  auto results = completer.complete("a");
  assert(results.size() >= 3);
  // "a" (1), "ab" (2), "abc" (3)
  assert(results[0].displayText() == "a");
  assert(results[0].description() == "A");
  assert(results[1].displayText() == "ab");
  assert(results[1].description() == "AB");
  assert(results[2].displayText() == "abc");
  assert(results[2].description() == "ABC");

  // Test duplicate handling
  results = completer.complete("dup");
  assert(results.size() == 1);
  assert(results[0].displayText() == "dup");
  assert(results[0].description() == "D1/D2");

  // Test case sensitivity (Uppercase prefix)
  // "A" should match "a", "ab", "abc" and capitalize them.
  results = completer.complete("A");
  assert(results.size() >= 3);
  assert(results[0].displayText() == "A");
  assert(results[0].description() == "A");
  assert(results[1].displayText() == "Ab");
  assert(results[1].description() == "AB");
  assert(results[2].displayText() == "Abc");
  assert(results[2].description() == "ABC");

  // Test sorting by length
  results = completer.complete("l");
  assert(results.size() >= 2);
  assert(results[0].displayText() == "long");
  assert(results[1].displayText() == "longer");

  std::filesystem::remove(testFile);
  std::cout << "All tests passed!" << std::endl;
}

int main() {
  testCompleter();
  return 0;
}
