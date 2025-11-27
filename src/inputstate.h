#ifndef INPUTSTATE_H_
#define INPUTSTATE_H_

#include <string>
#include <vector>
#include <optional>
#include <cmath>
#include "candidate.h"

namespace McFoxIM {
namespace InputState {

class InputState {
public:
    virtual ~InputState() = default;
};

class EmptyState : public InputState {
};

class CommittingState : public InputState {
public:
    explicit CommittingState(std::string commitString);
    const std::string &commitString() const { return commitString_; }

private:
    std::string commitString_;
};

class InputtingState : public InputState {
public:
    static const size_t CANDIDATES_PER_PAGE = 9;

    struct Args {
        size_t cursorIndex;
        std::string composingBuffer;
        std::vector<Candidate> candidates;
        std::optional<size_t> selectedCandidateIndex = std::nullopt;
    };

    explicit InputtingState(Args args);

    size_t cursorIndex() const { return cursorIndex_; }
    const std::string &composingBuffer() const { return composingBuffer_; }
    const std::vector<Candidate> &candidates() const { return candidates_; }
    std::optional<size_t> selectedCandidateIndex() const { return selectedCandidateIndex_; }

    const std::vector<Candidate> &candidatesInCurrentPage() const { return candidatesInCurrentPage_; }
    std::optional<size_t> selectedCandidateIndexInCurrentPage() const { return selectedCandidateIndexInCurrentPage_; }
    std::optional<size_t> candidatePageIndex() const { return candidatePageIndex_; }
    std::optional<size_t> candidatePageCount() const { return candidatePageCount_; }

private:
    size_t cursorIndex_;
    std::string composingBuffer_;
    std::vector<Candidate> candidates_;
    std::optional<size_t> selectedCandidateIndex_;

    std::vector<Candidate> candidatesInCurrentPage_;
    std::optional<size_t> selectedCandidateIndexInCurrentPage_;
    std::optional<size_t> candidatePageIndex_;
    std::optional<size_t> candidatePageCount_;
};

} // namespace InputState
} // namespace McFoxIM

#endif // INPUTSTATE_H_
