#ifndef BUMP_INPUTMANAGER_H
#define BUMP_INPUTMANAGER_H

#include "utils/vector.h"
#include "assets/light.h"
#include "movable.h"
#include <functional>
#include <memory>

class InputManager
{
public:
    InputManager()
    {
        state.fill(false);
    }
    void register_movement_listener(const std::shared_ptr<Movable>& listener);
    void send_input();
    void set_key(char c, bool state);

private:
    std::vector<std::shared_ptr<Movable>> movement_listeners;
    std::array<bool, 256> state;
};

extern InputManager inputManager;

#endif //BUMP_INPUTMANAGER_H
