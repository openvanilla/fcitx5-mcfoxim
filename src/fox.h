#ifndef FOX_H_
#define FOX_H_

#include <fcitx/addonfactory.h>
#include <fcitx/inputmethodengine.h>

namespace McFoxIM {

class FoxEngine : public fcitx::InputMethodEngineV2 {
public:
    FoxEngine(fcitx::Instance *instance);
    void keyEvent(const fcitx::InputMethodEntry &entry, fcitx::KeyEvent &keyEvent) override;
};

class FoxAddonFactory : public fcitx::AddonFactory {
public:
    fcitx::AddonInstance *create(fcitx::AddonManager *manager) override;
};

} // namespace McFoxIM

#endif // FOX_H_
