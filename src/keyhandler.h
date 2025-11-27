#ifndef KEYHANDLER_H_
#define KEYHANDLER_H_

#include <fcitx/event.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "completer.h"
#include "inputstate.h"

namespace McFoxIM {

class KeyHandler {
public:
    explicit KeyHandler(Completer &completer);

    using StateCallback = std::function<void(std::unique_ptr<InputState::InputState>)>;
    using ErrorCallback = std::function<void()>;

    bool handle(const fcitx::KeyEvent &keyEvent, const InputState::InputState &state,
                StateCallback stateCallback, ErrorCallback errorCallback);

private:
    Completer &completer_;
    std::string inputKeys_;
};

} // namespace McFoxIM

#endif // KEYHANDLER_H_
