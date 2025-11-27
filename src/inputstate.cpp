#include "inputstate.h"

#include <algorithm>

namespace McFoxIM {
namespace InputState {

CommittingState::CommittingState(std::string commitString)
    : commitString_(std::move(commitString)) {}

InputtingState::InputtingState(Args args)
    : cursorIndex_(args.cursorIndex),
      composingBuffer_(std::move(args.composingBuffer)),
      candidates_(std::move(args.candidates)),
      selectedCandidateIndex_(args.selectedCandidateIndex) {
  if (!candidates_.empty()) {
    size_t selectedIndex = selectedCandidateIndex_.value_or(0);
    selectedCandidateIndex_ = selectedIndex;

    candidatePageCount_ =
        (candidates_.size() + CANDIDATES_PER_PAGE - 1) / CANDIDATES_PER_PAGE;

    size_t pageIndex = selectedIndex / CANDIDATES_PER_PAGE;
    size_t startIndex = pageIndex * CANDIDATES_PER_PAGE;
    size_t endIndex =
        std::min(startIndex + CANDIDATES_PER_PAGE, candidates_.size());

    candidatesInCurrentPage_.reserve(endIndex - startIndex);
    for (size_t i = startIndex; i < endIndex; ++i) {
      candidatesInCurrentPage_.push_back(candidates_[i]);
    }

    selectedCandidateIndexInCurrentPage_ = selectedIndex % CANDIDATES_PER_PAGE;
    candidatePageIndex_ = pageIndex;
  }
}

}  // namespace InputState
}  // namespace McFoxIM
