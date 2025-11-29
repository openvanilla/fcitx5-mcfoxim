#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

#include "../src/inputstate.h"

using namespace McFoxIM;
using namespace McFoxIM::InputState;

void testEmptyState() {
  auto state = std::make_unique<EmptyState>();
  assert(dynamic_cast<EmptyState*>(state.get()) != nullptr);
  std::cout << "EmptyState test passed" << std::endl;
}

void testCommittingState() {
  std::string text = "test";
  auto state = std::make_unique<CommittingState>(text);
  assert(dynamic_cast<CommittingState*>(state.get()) != nullptr);
  assert(state->commitString() == text);
  std::cout << "CommittingState test passed" << std::endl;
}

void testInputtingState() {
  InputtingState::Args args;
  args.cursorIndex = 1;
  args.composingBuffer = "a";
  args.candidates = {Candidate("a", "A"), Candidate("b", "B")};
  args.selectedCandidateIndex = 0;

  auto state = std::make_unique<InputtingState>(args);
  assert(dynamic_cast<InputtingState*>(state.get()) != nullptr);
  assert(state->cursorIndex() == 1);
  assert(state->composingBuffer() == "a");
  assert(state->candidates().size() == 2);
  assert(state->selectedCandidateIndex() == 0);

  // Test pagination
  // Default page size is 9
  assert(state->candidatesInCurrentPage().size() == 2);
  assert(state->candidatePageIndex() == 0);
  assert(state->candidatePageCount() == 1);

  // Test with more candidates
  std::vector<Candidate> manyCandidates;
  for (int i = 0; i < 20; ++i) {
    manyCandidates.emplace_back(std::to_string(i), std::to_string(i));
  }
  args.candidates = manyCandidates;
  
  // Page 0
  args.selectedCandidateIndex = 0;
  state = std::make_unique<InputtingState>(args);
  assert(state->candidatesInCurrentPage().size() == 9);
  assert(state->candidatePageIndex() == 0);
  assert(state->candidatePageCount() == 3); // 20 / 9 = 2.22 -> 3 pages
  assert(state->selectedCandidateIndexInCurrentPage() == 0);

  // Page 1 (index 10 is in page 1: 9-17)
  args.selectedCandidateIndex = 10;
  state = std::make_unique<InputtingState>(args);
  assert(state->candidatesInCurrentPage().size() == 9);
  assert(state->candidatePageIndex() == 1);
  assert(state->selectedCandidateIndexInCurrentPage() == 1); // 10 - 9 = 1

  // Page 2 (index 19 is in page 2: 18-19)
  args.selectedCandidateIndex = 19;
  state = std::make_unique<InputtingState>(args);
  assert(state->candidatesInCurrentPage().size() == 2);
  assert(state->candidatePageIndex() == 2);
  assert(state->selectedCandidateIndexInCurrentPage() == 1); // 19 - 18 = 1

  std::cout << "InputtingState test passed" << std::endl;
}

int main() {
  testEmptyState();
  testCommittingState();
  testInputtingState();
  return 0;
}
