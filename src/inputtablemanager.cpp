#include "inputtablemanager.h"
#include <iostream>
#include <fstream>
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
    if (!std::filesystem::exists(dataPath_) || !std::filesystem::is_directory(dataPath_)) {
        return;
    }

    for (const auto &entry : std::filesystem::directory_iterator(dataPath_)) {
        if (entry.path().extension() == ".json") {
            // Peek into the file to get the name? 
            // Or just use filename as ID and load name on demand?
            // The requirement says "provide selectable input table list".
            // Let's try to read the "name" field from JSON without loading everything if possible,
            // or just load it. Since these are likely small config files, reading them might be okay.
            // But for performance, maybe just trust the filename or do a quick parse.
            
            std::ifstream f(entry.path());
            if (f.is_open()) {
                try {
                    nlohmann::json j;
                    f >> j;
                    std::string name = j.value("name", "");
                    if (!name.empty()) {
                        availableTables_.push_back({name, entry.path().string()});
                    }
                } catch (...) {
                    // Ignore malformed files
                }
            }
        }
    }
    
    // Sort by name or path?
    std::sort(availableTables_.begin(), availableTables_.end(), [](const TableInfo &a, const TableInfo &b) {
        return a.name < b.name;
    });
}

bool InputTableManager::setTable(const std::string &name) {
    auto it = std::find_if(availableTables_.begin(), availableTables_.end(), [&name](const TableInfo &info) {
        return info.name == name;
    });

    if (it != availableTables_.end()) {
        auto newTable = std::make_unique<InputTable>();
        if (newTable->load(it->path)) {
            currentTable_ = std::move(newTable);
            return true;
        }
    }
    return false;
}

const InputTable &InputTableManager::currentTable() const {
    if (currentTable_) {
        return *currentTable_;
    }
    return emptyTable_;
}

const std::vector<InputTableManager::TableInfo> &InputTableManager::availableTables() const {
    return availableTables_;
}

} // namespace McFoxIM
