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
      [](const Entry& e, const std::string& k) { return e.key < k; });

  // Iterate while keys match
  while (it != entries_.end() && it->key == key) {
    results.push_back(*it);
    ++it;
  }

  return results;
}

}  // namespace McFoxIM
