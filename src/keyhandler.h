#ifndef KEYHANDLER_H_
#define KEYHANDLER_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "completer.h"
#include "inputstate.h"
#include "key.h"

namespace McFoxIM {

class KeyHandler {
public:
    explicit KeyHandler(Completer &completer);

    using StateCallback = std::function<void(std::unique_ptr<InputState::InputState>)>;
    using ErrorCallback = std::function<void()>;

    bool handle(const Key &key, const InputState::InputState &state,
                StateCallback stateCallback, ErrorCallback errorCallback);

private:
    Completer &completer_;
    std::string inputKeys_;
};

} // namespace McFoxIM

#endif // KEYHANDLER_H_
