// Copyright (c) 2025 and onwards The McFoxxIM Authors.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "completer.h"

#include <algorithm>

namespace McFoxIM {

Completer::Completer(TableProvider provider) : provider_(std::move(provider)) {}

std::vector<Candidate> Completer::complete_(const std::string& prefix) {
  if (prefix.empty()) {
    return {};
  }
  const auto& table = provider_();
  const auto& data = table.entries();

  int left = 0;
  int right = static_cast<int>(data.size()) - 1;
  int firstMatch = -1;

  // Find the first entry that starts with the prefix
  while (left <= right) {
    int mid = left + (right - left) / 2;
    const auto& key = data[mid].phrase;

    if (key < prefix) {
      left = mid + 1;
    } else {
      bool startsWith = key.rfind(prefix, 0) == 0;
      if (startsWith) {
        firstMatch = mid;
        right = mid - 1;
      } else {
        right = mid - 1;
      }
    }
  }

  if (firstMatch == -1) {
    return {};
  }

  std::vector<Candidate> results;
  for (size_t i = firstMatch; i < data.size(); ++i) {
    const auto& key = data[i].phrase;
    if (key.rfind(prefix, 0) != 0) {
      break;
    }

    // Check duplicates
    bool duplicateFound = false;
    std::string keyLower = key;
    std::transform(keyLower.begin(), keyLower.end(), keyLower.begin(),
                   ::tolower);

    for (auto& result : results) {
      std::string resultLower = result.displayText();
      std::transform(resultLower.begin(), resultLower.end(),
                     resultLower.begin(), ::tolower);

      if (resultLower == keyLower) {
        result.appendDescription(data[i].description);
        duplicateFound = true;
        break;
      }
    }

    if (duplicateFound) {
      continue;
    }

    results.emplace_back(key, data[i].description);
  }

  return results;
}

std::vector<Candidate> Completer::complete(const std::string& prefix) {
  if (prefix.empty()) {
    return {};
  }

  std::vector<Candidate> result;
  if (std::isupper(static_cast<unsigned char>(prefix[0]))) {
    auto original = complete_(prefix);
    std::string lowerPrefix = prefix;
    std::transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(),
                   ::tolower);
    auto lowered = complete_(lowerPrefix);

    result = std::move(original);
    for (const auto& c : lowered) {
      std::string text = c.displayText();
      if (!text.empty()) {
        text[0] = std::toupper(static_cast<unsigned char>(text[0]));
      }
      result.emplace_back(text, c.description());
    }
  } else {
    result = complete_(prefix);
  }

  std::sort(result.begin(), result.end(),
            [](const Candidate& a, const Candidate& b) {
              return a.displayText().length() < b.displayText().length();
            });
  return result;
}

}  // namespace McFoxIM
