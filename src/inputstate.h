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

#ifndef INPUTSTATE_H_
#define INPUTSTATE_H_

#include <cmath>
#include <optional>
#include <string>
#include <vector>

#include "candidate.h"

namespace McFoxIM {
namespace InputState {

class InputState {
 public:
  virtual ~InputState() = default;
};

class EmptyState : public InputState {};

class CommittingState : public InputState {
 public:
  explicit CommittingState(std::string commitString);
  const std::string& commitString() const { return commitString_; }

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
  const std::string& composingBuffer() const { return composingBuffer_; }
  const std::vector<Candidate>& candidates() const { return candidates_; }
  std::optional<size_t> selectedCandidateIndex() const {
    return selectedCandidateIndex_;
  }

  const std::vector<Candidate>& candidatesInCurrentPage() const {
    return candidatesInCurrentPage_;
  }
  std::optional<size_t> selectedCandidateIndexInCurrentPage() const {
    return selectedCandidateIndexInCurrentPage_;
  }
  std::optional<size_t> candidatePageIndex() const {
    return candidatePageIndex_;
  }
  std::optional<size_t> candidatePageCount() const {
    return candidatePageCount_;
  }

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

}  // namespace InputState
}  // namespace McFoxIM

#endif  // INPUTSTATE_H_
