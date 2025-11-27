#include "candidate.h"

namespace McFoxIM {

Candidate::Candidate(std::string displayText, std::string description)
    : displayText_(std::move(displayText)), description_(std::move(description)) {}

} // namespace McFoxIM
