#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include <fcitx/event.h>
#include <fcitx/inputcontext.h> // For KeyEvent constructor if needed, though we pass nullptr

#include "../src/keyhandler.h"
#include "../src/inputtable.h"
#include "../src/completer.h"

using namespace McFoxIM;

// Helper to create a test table
void createTestTable(const std::string& filename) {
  std::ofstream out(filename);
  out << R"({
        "name": "TestTable",
        "data": [
            ["a", "A"],
            ["b", "B"],
            ["c", "C"]
        ]
    })";
  out.close();
}

void testKeyHandler() {
  std::string testFile = "test_keyhandler_data.json";
  createTestTable(testFile);

  InputTable table;
  bool loaded = table.load(testFile);
  assert(loaded);

  Completer completer([&table]() -> const InputTable& { return table; });
  KeyHandler handler(completer);

  // Helper to run handle
  auto runHandle = [&](const fcitx::KeyEvent& event, const InputState::InputState& state) 
      -> std::unique_ptr<InputState::InputState> {
    std::unique_ptr<InputState::InputState> resultState = nullptr;
    handler.handle(event, state, 
      [&](std::unique_ptr<InputState::InputState> s) { resultState = std::move(s); },
      []() { /* Error callback */ }
    );
    return resultState;
  };

  // Test 1: EmptyState + 'a' -> InputtingState
  {
    InputState::EmptyState state;
    // KeyEvent(InputContext *ic, Key key, bool isRelease)
    fcitx::KeyEvent event(nullptr, fcitx::Key("a"), false);
    auto newState = runHandle(event, state);
    
    assert(newState != nullptr);
    auto inputState = dynamic_cast<InputState::InputtingState*>(newState.get());
    assert(inputState != nullptr);
    assert(inputState->composingBuffer() == "a");
    assert(inputState->cursorIndex() == 1);
  }

  // Test 2: EmptyState + 'z' (not in table) -> InputtingState (handled, even if no candidates)
  {
    InputState::EmptyState state;
    fcitx::KeyEvent event(nullptr, fcitx::Key("z"), false);
    std::unique_ptr<InputState::InputState> resultState = nullptr;
    bool handled = handler.handle(event, state, [&](auto s){ resultState = std::move(s); }, [](){});
    assert(handled);
    assert(resultState != nullptr);
    auto inputState = dynamic_cast<InputState::InputtingState*>(resultState.get());
    assert(inputState != nullptr);
    assert(inputState->composingBuffer() == "z");
    assert(inputState->candidates().empty());
  }

  // Test 3: InputtingState("a") + 'b' -> InputtingState("ab")
  {
    InputState::InputtingState::Args args;
    args.cursorIndex = 1;
    args.composingBuffer = "a";
    args.candidates = {Candidate("a", "A")};
    InputState::InputtingState state(args);

    fcitx::KeyEvent event(nullptr, fcitx::Key("b"), false);
    auto newState = runHandle(event, state);

    assert(newState != nullptr);
    auto inputState = dynamic_cast<InputState::InputtingState*>(newState.get());
    assert(inputState != nullptr);
    assert(inputState->composingBuffer() == "ab");
    assert(inputState->cursorIndex() == 2);
  }

  // Test 4: InputtingState("a") + Space -> CommittingState("A") (if selected) or InputtingState (if multiple pages)
  // With "a" -> "A", only 1 candidate. Space should commit "A " (with space? Logic says commitString + " " usually)
  // Let's check logic:
  // if key is space:
  //   if cursorIndex == 0 -> commit " "
  //   else -> insert space? No, usually space selects first candidate if composing.
  // Wait, looking at code:
  // if (key.check(fcitx::Key(FcitxKey_space))) {
  //   if (inputState->cursorIndex() == 0) ...
  //   std::string newComposingBuffer = inputState->composingBuffer();
  //   newComposingBuffer.insert(inputState->cursorIndex(), " ");
  // ...
  // It seems space inserts a space into composing buffer in this implementation!
  // Unless it's handled by "Number keys selection" or "Tab or Return".
  // Let's check "Tab or Return to commit".
  
  // Test 5: InputtingState("a") + Return -> CommittingState("A ")
  {
    InputState::InputtingState::Args args;
    args.cursorIndex = 1;
    args.composingBuffer = "a";
    args.candidates = {Candidate("a", "A")};
    args.selectedCandidateIndex = 0;
    InputState::InputtingState state(args);

    fcitx::KeyEvent event(nullptr, fcitx::Key(FcitxKey_Return), false);
    auto newState = runHandle(event, state);

    assert(newState != nullptr);
    auto commitState = dynamic_cast<InputState::CommittingState*>(newState.get());
    assert(commitState != nullptr);
    // Logic: selectedCandidate.displayText() + " "
    assert(commitState->commitString() == "a "); 
  }

  std::filesystem::remove(testFile);
  std::cout << "KeyHandler test passed" << std::endl;
}

int main() {
  testKeyHandler();
  return 0;
}
