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

#include "keyhandler.h"

#include <fcitx/inputmethodentry.h>

#include <iostream>

namespace McFoxIM {

KeyHandler::KeyHandler(Completer& completer) : completer_(completer) {
  inputKeys_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'^";
}

bool KeyHandler::handle(const fcitx::KeyEvent& keyEvent,
                        const InputState::InputState& state,
                        StateCallback stateCallback,
                        ErrorCallback errorCallback) {
  if (keyEvent.isRelease()) {
    return false;
  }

  auto key = keyEvent.key();
  std::string ascii;
  if (key.isSimple()) {
    ascii = key.toString();
  }

  // Check if state is EmptyState
  if (dynamic_cast<const InputState::EmptyState*>(&state)) {
    if (ascii.length() == 1 && inputKeys_.find(ascii) == std::string::npos) {
      return false;
    }
  }

  if (ascii.length() == 1 && inputKeys_.find(ascii) != std::string::npos) {
    std::string chr = ascii;
    if (chr == "'") {
      chr = "’";
    }

    if (dynamic_cast<const InputState::EmptyState*>(&state)) {
      std::string newComposingBuffer = chr;
      auto candidates = completer_.complete(newComposingBuffer);

      InputState::InputtingState::Args args;
      args.cursorIndex = newComposingBuffer.length();
      args.composingBuffer = newComposingBuffer;
      args.candidates = candidates;
      if (!candidates.empty()) {
        args.selectedCandidateIndex = 0;
      }

      stateCallback(std::make_unique<InputState::InputtingState>(args));
      return true;
    } else if (auto inputState =
                   dynamic_cast<const InputState::InputtingState*>(&state)) {
      std::string newComposingBuffer = inputState->composingBuffer();
      newComposingBuffer.insert(inputState->cursorIndex(), chr);

      size_t newCursorIndex = inputState->cursorIndex() + 1;
      auto candidates = completer_.complete(newComposingBuffer);

      InputState::InputtingState::Args args;
      args.cursorIndex = newCursorIndex;
      args.composingBuffer = newComposingBuffer;
      args.candidates = candidates;
      if (!candidates.empty()) {
        args.selectedCandidateIndex = 0;
      }

      stateCallback(std::make_unique<InputState::InputtingState>(args));
      return true;
    }
  }

  if (auto inputState =
          dynamic_cast<const InputState::InputtingState*>(&state)) {
    // Tab or Return to commit
    if (key.check(fcitx::Key(FcitxKey_Tab)) ||
        key.check(fcitx::Key(FcitxKey_Return))) {
      if (!inputState->candidates().empty()) {
        size_t index = inputState->selectedCandidateIndex().value_or(0);
        if (index < inputState->candidates().size()) {
          const auto& selectedCandidate = inputState->candidates()[index];
          stateCallback(std::make_unique<InputState::CommittingState>(
              selectedCandidate.displayText() + " "));
        }
      } else {
        stateCallback(std::make_unique<InputState::CommittingState>(
            inputState->composingBuffer()));
      }
      return true;
    }

    // Number keys selection
    if (ascii.length() == 1 && ascii >= "1" && ascii <= "9") {
      const auto& candidates = inputState->candidatesInCurrentPage();
      if (candidates.empty()) {
        errorCallback();
      } else {
        int index = std::stoi(ascii) - 1;
        if (index >= 0 && index < static_cast<int>(candidates.size())) {
          const auto& selectedCandidate = candidates[index];
          stateCallback(std::make_unique<InputState::CommittingState>(
              selectedCandidate.displayText()));
        } else {
          errorCallback();
          return true;
        }
      }
      return true;
    }

    if (key.check(fcitx::Key(FcitxKey_space))) {
      if (inputState->cursorIndex() == 0) {
        stateCallback(std::make_unique<InputState::CommittingState>(" "));

        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex();
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = inputState->selectedCandidateIndex();
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      }

      std::string newComposingBuffer = inputState->composingBuffer();
      newComposingBuffer.insert(inputState->cursorIndex(), " ");

      size_t newCursorIndex = inputState->cursorIndex() + 1;
      auto candidates = completer_.complete(newComposingBuffer);

      InputState::InputtingState::Args args;
      args.cursorIndex = newCursorIndex;
      args.composingBuffer = newComposingBuffer;
      args.candidates = candidates;
      if (!candidates.empty()) {
        args.selectedCandidateIndex = 0;
      }

      stateCallback(std::make_unique<InputState::InputtingState>(args));
      return true;
    }

    if (key.check(fcitx::Key(FcitxKey_Escape))) {
      return true;
    }

    if (key.check(fcitx::Key(FcitxKey_BackSpace))) {
      if (inputState->cursorIndex() > 0) {
        std::string newComposingBuffer = inputState->composingBuffer();
        newComposingBuffer.erase(inputState->cursorIndex() - 1, 1);
        size_t newCursorIndex = inputState->cursorIndex() - 1;

        if (newComposingBuffer.empty()) {
          stateCallback(std::make_unique<InputState::EmptyState>());
          return true;
        } else {
          if (newComposingBuffer[0] == ' ') {
            newComposingBuffer.erase(0, 1);
            if (newComposingBuffer.empty()) {
              stateCallback(std::make_unique<InputState::EmptyState>());
              return true;
            } else {
              stateCallback(std::make_unique<InputState::CommittingState>(" "));
            }
          }

          auto candidates = completer_.complete(newComposingBuffer);
          InputState::InputtingState::Args args;
          args.cursorIndex = newCursorIndex;
          args.composingBuffer = newComposingBuffer;
          args.candidates = candidates;
          if (!candidates.empty()) {
            args.selectedCandidateIndex = 0;
          }
          stateCallback(std::make_unique<InputState::InputtingState>(args));
          return true;
        }
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Delete))) {
      if (inputState->cursorIndex() < inputState->composingBuffer().length()) {
        std::string newComposingBuffer = inputState->composingBuffer();
        newComposingBuffer.erase(inputState->cursorIndex(), 1);
        size_t newCursorIndex = inputState->cursorIndex();

        if (newComposingBuffer.empty()) {
          stateCallback(std::make_unique<InputState::EmptyState>());
          return true;
        } else {
          if (newComposingBuffer[0] == ' ') {
            newComposingBuffer.erase(0, 1);
            if (newComposingBuffer.empty()) {
              stateCallback(std::make_unique<InputState::EmptyState>());
              return true;
            } else {
              stateCallback(std::make_unique<InputState::CommittingState>(" "));
            }
          }

          auto candidates = completer_.complete(newComposingBuffer);
          InputState::InputtingState::Args args;
          args.cursorIndex = newCursorIndex;
          args.composingBuffer = newComposingBuffer;
          args.candidates = candidates;
          if (!candidates.empty()) {
            args.selectedCandidateIndex = 0;
          }
          stateCallback(std::make_unique<InputState::InputtingState>(args));
          return true;
        }
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Left))) {
      if (inputState->cursorIndex() > 0) {
        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex() - 1;
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = inputState->selectedCandidateIndex();
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Right))) {
      if (inputState->cursorIndex() < inputState->composingBuffer().length()) {
        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex() + 1;
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = inputState->selectedCandidateIndex();
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Up))) {
      if (!inputState->candidates().empty()) {
        size_t current = inputState->selectedCandidateIndex().value_or(0);
        size_t count = inputState->candidates().size();
        size_t newIndex = (current + count - 1) % count;

        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex();
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = newIndex;
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Down))) {
      if (!inputState->candidates().empty()) {
        size_t current = inputState->selectedCandidateIndex().value_or(0);
        size_t count = inputState->candidates().size();
        size_t newIndex = (current + 1) % count;

        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex();
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = newIndex;
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Page_Down))) {
      if (!inputState->candidates().empty()) {
        size_t candidatesPerPage =
            InputState::InputtingState::CANDIDATES_PER_PAGE;
        size_t current = inputState->selectedCandidateIndex().value_or(0);
        size_t count = inputState->candidates().size();

        size_t newIndex =
            ((current / candidatesPerPage) + 1) * candidatesPerPage;
        newIndex = std::min(newIndex, count - 1);

        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex();
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = newIndex;
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Page_Up))) {
      if (!inputState->candidates().empty()) {
        size_t candidatesPerPage =
            InputState::InputtingState::CANDIDATES_PER_PAGE;
        size_t current = inputState->selectedCandidateIndex().value_or(0);

        size_t currentPage = current / candidatesPerPage;
        size_t newIndex = 0;
        if (currentPage > 0) {
          newIndex = (currentPage - 1) * candidatesPerPage;
        }

        InputState::InputtingState::Args args;
        args.cursorIndex = inputState->cursorIndex();
        args.composingBuffer = inputState->composingBuffer();
        args.candidates = inputState->candidates();
        args.selectedCandidateIndex = newIndex;
        stateCallback(std::make_unique<InputState::InputtingState>(args));
        return true;
      } else {
        errorCallback();
        return true;
      }
    }

    if (key.check(fcitx::Key(FcitxKey_Home))) {
      if (inputState->cursorIndex() == 0) {
        errorCallback();
        return true;
      }
      InputState::InputtingState::Args args;
      args.cursorIndex = 0;
      args.composingBuffer = inputState->composingBuffer();
      args.candidates = inputState->candidates();
      args.selectedCandidateIndex = inputState->selectedCandidateIndex();
      stateCallback(std::make_unique<InputState::InputtingState>(args));
      return true;
    }

    if (key.check(fcitx::Key(FcitxKey_End))) {
      if (inputState->cursorIndex() == inputState->composingBuffer().length()) {
        errorCallback();
        return true;
      }
      InputState::InputtingState::Args args;
      args.cursorIndex = inputState->composingBuffer().length();
      args.composingBuffer = inputState->composingBuffer();
      args.candidates = inputState->candidates();
      args.selectedCandidateIndex = inputState->selectedCandidateIndex();
      stateCallback(std::make_unique<InputState::InputtingState>(args));
      return true;
    }

    // Printable characters
    if (ascii.length() == 1) {
      std::string commitString = inputState->composingBuffer();
      commitString.insert(inputState->cursorIndex(), ascii);
      stateCallback(
          std::make_unique<InputState::CommittingState>(commitString));
      return true;
    }
  }

  errorCallback();
  return false;
}

}  // namespace McFoxIM
