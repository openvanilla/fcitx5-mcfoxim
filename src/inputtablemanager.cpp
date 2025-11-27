#include "inputtablemanager.h"

#include <fcitx-utils/log.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace McFoxIM {

InputTableManager::InputTableManager(std::string dataPath)
    : dataPath_(std::move(dataPath)) {
  scanTables();
  // Ensure currentTable_ is never null
  currentTable_ = std::make_unique<InputTable>();
}

void InputTableManager::scanTables() {
  availableTables_.clear();
  if (!std::filesystem::exists(dataPath_) ||
      !std::filesystem::is_directory(dataPath_)) {
    FCITX_INFO() << "Data path does not exist or is not a directory: "
                 << dataPath_;
    return;
  }
  FCITX_INFO() << "Scanning tables in: " << dataPath_;

  for (const auto& entry : std::filesystem::directory_iterator(dataPath_)) {
    if (entry.path().extension() == ".json") {
      FCITX_INFO() << "Found JSON file: " << entry.path();
      // Peek into the file to get the name?
      // Or just use filename as ID and load name on demand?
      // The requirement says "provide selectable input table list".
      // Let's try to read the "name" field from JSON without loading everything
      // if possible, or just load it. Since these are likely small config
      // files, reading them might be okay. But for performance, maybe just
      // trust the filename or do a quick parse.

      std::ifstream f(entry.path());
      if (f.is_open()) {
        try {
          nlohmann::json j;
          f >> j;
          std::string name = j.value("name", "");
          if (!name.empty()) {
            availableTables_.push_back({name, entry.path().string()});
            FCITX_INFO() << "Found table: " << name;
          }
        } catch (...) {
          FCITX_INFO() << "Failed to parse JSON: " << entry.path();
          // Ignore malformed files
        }
      }
    }
  }

  // Sort by name or path?
  std::sort(
      availableTables_.begin(), availableTables_.end(),
      [](const TableInfo& a, const TableInfo& b) { return a.name < b.name; });
}

bool InputTableManager::setTable(const std::string& name) {
  auto it = std::find_if(
      availableTables_.begin(), availableTables_.end(),
      [&name](const TableInfo& info) { return info.name == name; });
  FCITX_INFO() << "Setting table to: " << name;

  if (it != availableTables_.end()) {
    auto newTable = std::make_unique<InputTable>();
    FCITX_INFO() << "Attempting to load table from path: " << it->path;
    FCITX_INFO() << "Table name: " << it->name;
    if (newTable->load(it->path)) {
      currentTable_ = std::move(newTable);
      FCITX_INFO() << "Successfully loaded table: " << name;
      return true;
    } else {
      FCITX_INFO() << "Failed to load table: " << name << " from " << it->path;
    }
  } else {
    FCITX_INFO() << "Table not found in available list: " << name;
  }
  return false;
}

const InputTable& InputTableManager::currentTable() const {
  if (currentTable_) {
    return *currentTable_;
  }
  return emptyTable_;
}

const std::vector<InputTableManager::TableInfo>&
InputTableManager::availableTables() const {
  return availableTables_;
}

}  // namespace McFoxIM
