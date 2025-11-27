#ifndef INPUTTABLE_H_
#define INPUTTABLE_H_

#include <string>
#include <vector>

namespace McFoxIM {

class InputTable {
 public:
  struct Entry {
    std::string phrase;
    std::string description;
  };

  bool load(const std::string& path);
  std::vector<Entry> getCandidates(const std::string& key) const;
  const std::string& name() const { return name_; }
  const std::vector<Entry>& entries() const { return entries_; }

 private:
  std::string name_;
  std::vector<Entry> entries_;
};

}  // namespace McFoxIM

#endif  // INPUTTABLE_H_
