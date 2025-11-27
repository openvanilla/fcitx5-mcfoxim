#ifndef INPUTTABLEMANAGER_H_
#define INPUTTABLEMANAGER_H_

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "inputtable.h"

namespace McFoxIM {

class InputTableManager {
public:
    struct TableInfo {
        std::string name;
        std::string path;
    };

    InputTableManager(std::string dataPath);

    bool setTable(const std::string &name);
    const InputTable &currentTable() const;
    const std::vector<TableInfo> &availableTables() const;

private:
    void scanTables();

    std::string dataPath_;
    std::unique_ptr<InputTable> currentTable_;
    std::vector<TableInfo> availableTables_;
    InputTable emptyTable_; // Fallback empty table
};

} // namespace McFoxIM

#endif // INPUTTABLEMANAGER_H_
