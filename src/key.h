#ifndef KEY_H_
#define KEY_H_

#include <string>

namespace McFoxIM {

enum class KeyName {
    UNKNOWN,
    TAB,
    RETURN,
    SPACE,
    ESC,
    BACKSPACE,
    DELETE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    PAGE_DOWN,
    PAGE_UP,
    HOME,
    END
};

struct Key {
    std::string ascii;
    KeyName name;
};

} // namespace McFoxIM

#endif // KEY_H_
