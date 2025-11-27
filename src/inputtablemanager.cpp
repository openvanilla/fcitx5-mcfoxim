#include "inputtablemanager.h"

#include <fcitx-utils/log.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include <iomanip>
#include <sstream>

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

  for (int i = 0; i <= 43; ++i) {
    std::stringstream ss;
    ss << "TW_" << std::setw(2) << std::setfill('0') << i << ".json";
    std::string filename = ss.str();
    std::filesystem::path filePath = std::filesystem::path(dataPath_) / filename;

    if (std::filesystem::exists(filePath)) {
      FCITX_INFO() << "Found JSON file: " << filePath;
      TableInfo info;
      info.id = filePath.stem().string();
      info.path = filePath.string();
      
      std::ifstream f(filePath);
      if (f.is_open()) {
        try {
            nlohmann::json j;
            f >> j;
            info.name = j.value("name", info.id);
        } catch (...) {
            info.name = info.id;
        }
      } else {
          info.name = info.id;
      }
      
      availableTables_.push_back(info);
    }
  }
}

bool InputTableManager::setTable(int index) {
  if (index >= 0 && index < static_cast<int>(availableTables_.size())) {
    const auto& info = availableTables_[index];
    FCITX_INFO() << "Setting table to index: " << index << ", id: " << info.id;

    auto newTable = std::make_unique<InputTable>();
    FCITX_INFO() << "Attempting to load table from path: " << info.path;
    if (newTable->load(info.path)) {
      currentTable_ = std::move(newTable);
      FCITX_INFO() << "Successfully loaded table: " << info.name;
      return true;
    } else {
      FCITX_INFO() << "Failed to load table: " << info.name << " from " << info.path;
    }
  } else {
    FCITX_INFO() << "Invalid table index: " << index;
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
