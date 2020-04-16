#ifndef BUMP_INPUTMANAGER_H
#define BUMP_INPUTMANAGER_H

#include "vector.h"
#include "movable.h"
#include <functional>
#include <memory>

class InputManager
{
public:
    void register_movement_listener(std::shared_ptr<Movable> listener);
    void send_movement(mygl::Vec3 translation);

private:
    std::vector<std::shared_ptr<Movable>> movement_listeners;
};

extern InputManager inputManager;

#endif //BUMP_INPUTMANAGER_H
