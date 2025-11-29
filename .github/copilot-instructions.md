# fcitx5-mcfoxim

This document provides a high-level overview of the `fcitx5-mcfoxim` project, intended to give an AI assistant the context needed to understand its architecture, components, and purpose.

## 1. Project Overview

`fcitx5-mcfoxim` is an input method engine for the Fcitx5 input framework on Linux. Its primary purpose is to provide input methods for various indigenous languages of Taiwan. It utilizes linguistic data provided by the Indigenous Languages Research and Development Foundation (原語會). The project is written in C++17 and uses the CMake build system.

## 2. Core Architecture and Data Flow

The engine follows a state machine pattern to manage user input. The core logic is orchestrated by `FoxEngine`, which responds to events from the Fcitx5 framework.

The typical data flow for a key press is as follows:

1.  Fcitx5 sends a `keyEvent` to the active `FoxEngine` instance.
2.  The `FoxEngine` passes the key event to the `KeyHandler`.
3.  The `KeyHandler` inspects the current `InputState` (e.g., `InputtingState`, `EmptyState`) and the key press to determine the next logical state.
4.  It returns a new `InputState` object to `FoxEngine`. This could be a new `InputtingState` with an updated composing buffer, a `CommittingState` to commit the selected candidate, or an `EmptyState` after resetting.
5.  `FoxEngine` transitions to the new state and updates the UI (e.g., pre-edit text, candidate list) accordingly.

### Key Components (in `src/`)

- **`FoxEngine` (`fox.h`/`.cpp`):** The central class that implements the `fcitx::InputMethodEngineV2` interface. It holds instances of all other components and manages the overall lifecycle and state transitions.
- **`InputState` (`inputstate.h`/`.cpp`):** An abstract base class representing the state of the input context. Key subclasses are:
  - `EmptyState`: No active composition.
  - `InputtingState`: The user is typing, and a candidate list may be visible. Holds the composing buffer, candidates, and cursor position.
  - `CommittingState`: A candidate has been selected, and its text is ready to be committed to the client application.
- **`KeyHandler` (`keyhandler.h`/`.cpp`):** A stateless component responsible for processing key events. Its main job is to take the current state and a key event and produce the next state.
- **`Completer` (`completer.h`/`.cpp`):** Generates a list of `Candidate` objects based on the current composing buffer by querying the `InputTableManager`.
- **`InputTableManager` (`inputtablemanager.h`/`.cpp`):** Manages the loading and querying of linguistic data. It reads the `.json` files from disk and provides an interface for the `Completer` to find matching words and phrases.
- **`Candidate` (`candidate.h`/`.cpp`):** A simple data structure representing a single candidate word or phrase in the suggestion list.

## 3. File Structure

- `src/`: Contains all the C++ source code for the input method engine.
- `data/`: Contains the linguistic data files (`.json`) and icon assets.
- `tests/`: Contains unit tests for the project components (e.g., `test_completer.cpp`).
- `tools/`: Contains helper scripts. `convert.py` is used to process glossary data into the JSON format used by the engine.
- `.github/`: CI/CD workflows, primarily for building and testing on GitHub Actions.
- `CMakeLists.txt`: The main CMake build script. It defines the project, finds dependencies, and includes the subdirectories.

## 4. Data and Configuration

- **`.json` files (in `data/`):** These are the core data tables for the input method. Each file contains a mapping of input sequences (keys) to candidate phrases for a specific language variant.
- **`.conf` files (in `src/`):** These are Fcitx5 configuration files.
  - `fox.conf`: The main addon configuration file.
  - `fox_TW_*.conf`: Configuration files for each specific language variant, defining its name, icon, and the data file it should use.

## 5. Build System

The project is built using CMake. Key dependencies include:

- Fcitx5 core libraries (`Fcitx5Core`, `Fcitx5Config`, `Fcitx5Utils`)
- `nlohmann/json`: A C++ library for parsing the JSON data files, which is fetched automatically by CMake via `FetchContent`.

The main output is `libfox.so`, a shared library that Fcitx5 loads as an input method addon.
