#include "inputmanager.h"

InputManager inputManager;

void InputManager::send_movement(mygl::Vec3 translation)
{
    for (auto c : movement_listeners)
    {
        c->translate(translation);
    }
}

void InputManager::register_movement_listener(std::shared_ptr<Movable> listener)
{
    movement_listeners.push_back(listener);
}
