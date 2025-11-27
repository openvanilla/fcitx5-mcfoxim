#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <string>

namespace McFoxIM {

class Candidate {
public:
    Candidate(std::string displayText, std::string description);

    const std::string &displayText() const { return displayText_; }
    const std::string &description() const { return description_; }

private:
    std::string displayText_;
    std::string description_;
};

} // namespace McFoxIM

#endif // CANDIDATE_H_
