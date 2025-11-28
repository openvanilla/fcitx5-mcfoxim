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

#include "inputtable.h"

#include <fcitx-utils/log.h>

#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace McFoxIM {

bool InputTable::load(const std::string& path) {
  std::ifstream f(path);
  if (!f.is_open()) {
    FCITX_INFO() << "Failed to open file: " << path;
    return false;
  }

  try {
    json j;
    f >> j;

    name_ = j.value("name", "");
    entries_.clear();

    if (j.contains("data") && j["data"].is_array()) {
      for (const auto& item : j["data"]) {
        if (item.is_array() && item.size() >= 2) {
          entries_.push_back(
              {item[0].get<std::string>(), item[1].get<std::string>()});
        }
      }
    }
    FCITX_INFO() << "Loaded " << entries_.size() << " entries from " << path;
  } catch (...) {
    FCITX_INFO() << "Exception while loading " << path;
    return false;
  }
  return true;
}

std::vector<InputTable::Entry> InputTable::getCandidates(
    const std::string& key) const {
  std::vector<InputTable::Entry> results;

  // Binary search for the first element that is not less than key
  auto it = std::lower_bound(
      entries_.begin(), entries_.end(), key,
      [](const Entry& e, const std::string& k) { return e.phrase < k; });

  // Iterate while keys match
  while (it != entries_.end() && it->phrase == key) {
    results.push_back(*it);
    ++it;
  }

  return results;
}

}  // namespace McFoxIM
