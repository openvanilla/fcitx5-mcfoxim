// Copyright (c) 2025 and onwards The McFoxxIM Authors.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

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
