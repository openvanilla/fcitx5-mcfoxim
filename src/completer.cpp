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
    const auto& key = data[mid].phrase;

    if (key < prefix) {
      bool startsWith = key.rfind(prefix, 0) == 0;

      if (key < prefix && !startsWith) {
        left = mid + 1;
      } else if (startsWith) {
        firstMatch = mid;
        right = mid - 1;
      } else {
        right = mid - 1;
      }
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

  left = 0;
  right = static_cast<int>(data.size()) - 1;
  firstMatch = -1;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    const auto& key = data[mid].phrase;

    bool startsWith = key.size() >= prefix.size() &&
                      key.compare(0, prefix.size(), prefix) == 0;

    if (key < prefix && !startsWith) {
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
    const auto& key = data[i].phrase;
    bool startsWith = key.size() >= prefix.size() &&
                      key.compare(0, prefix.size(), prefix) == 0;
    if (startsWith) {
      results.emplace_back(data[i].phrase, data[i].description);
    } else {
      break;
    }
  }

  return results;
}

}  // namespace McFoxIM
