#include "completer.h"

#include <algorithm>

namespace McFoxIM {

Completer::Completer(TableProvider provider) : provider_(std::move(provider)) {}

std::vector<Candidate> Completer::complete(const std::string& prefix) {
  const auto& table = provider_();
  const auto& data = table.entries();

  if (data.empty()) {
    return {};
  }

  int left = 0;
  int right = static_cast<int>(data.size()) - 1;
  int firstMatch = -1;

  // Find the first entry that starts with the prefix
  while (left <= right) {
    int mid = left + (right - left) / 2;
    const auto& key = data[mid].key;

    if (key < prefix) {
      // Check if key starts with prefix is not possible if key < prefix
      // BUT we need to be careful about string comparison.
      // "a" < "ab", but "ab" starts with "a".
      // If key is "a" and prefix is "ab", key < prefix.
      // If key is "ac" and prefix is "ab", key > prefix.

      // The logic in TS:
      // if (data[mid][0] < prefix) { left = mid + 1; }
      // else if (data[mid][0].startsWith(prefix)) { firstMatch = mid; right =
      // mid - 1; } else { right = mid - 1; }

      // In C++:
      // std::string comparison is lexicographical.
      // If prefix is "foo", "fo" < "foo". "fo" does not start with "foo".
      // "foobar" > "foo". "foobar" starts with "foo".

      // We need to check startsWith first or handle the logic carefully.
      // The TS logic relies on the fact that if it starts with prefix, it is >=
      // prefix. But "foobar" > "foo".

      // Let's implement startsWith helper or use rfind.
      bool startsWith = key.rfind(prefix, 0) == 0;

      if (key < prefix && !startsWith) {
        // Strictly less and not a prefix match (which is impossible if strictly
        // less, unless prefix is empty?) If prefix is "a", key "a" is not <
        // "a". If prefix is "ab", key "a" < "ab". "a" does not start with "ab".
        left = mid + 1;
      } else if (startsWith) {
        firstMatch = mid;
        right = mid - 1;
      } else {
        // key > prefix and does not start with prefix
        right = mid - 1;
      }
    } else {
      // key >= prefix
      bool startsWith = key.rfind(prefix, 0) == 0;
      if (startsWith) {
        firstMatch = mid;
        right = mid - 1;
      } else {
        right = mid - 1;
      }
    }
  }

  // Let's stick closer to the TS logic structure to be safe
  /*
    if (data[mid][0] < prefix) {
      left = mid + 1;
    } else if (data[mid][0].startsWith(prefix)) {
      firstMatch = mid;
      right = mid - 1; // Continue searching left for the first match
    } else {
      right = mid - 1;
    }
  */

  // Re-implementing the loop exactly as TS
  left = 0;
  right = static_cast<int>(data.size()) - 1;
  firstMatch = -1;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    const auto& key = data[mid].key;

    // Check startsWith
    bool startsWith = key.size() >= prefix.size() &&
                      key.compare(0, prefix.size(), prefix) == 0;

    if (key < prefix && !startsWith) {
      // Note: if key starts with prefix, it must be >= prefix (e.g. "apple" >=
      // "app"). So if key < prefix, it cannot start with prefix. So the
      // !startsWith check is actually redundant if we trust string ordering
      // properties, but let's keep it implicit or explicit. Actually, "app" <
      // "apple". "apple" starts with "app". Wait, if prefix is "app", and key
      // is "apple". "apple" > "app". If prefix is "apple", and key is "app".
      // "app" < "apple".

      // TS: if (data[mid][0] < prefix)
      // If key="app", prefix="apple". "app" < "apple". "app" does not start
      // with "apple". -> left = mid + 1. Correct.
      left = mid + 1;
    } else if (startsWith) {
      firstMatch = mid;
      right = mid - 1;
    } else {
      right = mid - 1;
    }
  }

  if (firstMatch == -1) {
    return {};
  }

  std::vector<Candidate> results;
  for (size_t i = firstMatch; i < data.size(); ++i) {
    const auto& key = data[i].key;
    bool startsWith = key.size() >= prefix.size() &&
                      key.compare(0, prefix.size(), prefix) == 0;
    if (startsWith) {
      results.emplace_back(data[i].key, data[i].value);
    } else {
      break;
    }
  }

  return results;
}

}  // namespace McFoxIM
