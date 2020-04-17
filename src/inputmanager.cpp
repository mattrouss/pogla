#include "inputmanager.h"

InputManager inputManager;

void InputManager::send_input()
{
    mygl::Vec3 translation = {{0,0,0}};
    if (state['a'] or state['q'])
    {
        translation += {{-1,0,0}};
    }
    if (state['z'] or state['w'])
    {
        translation += {{0,0,1}};
    }
    if (state['s'])
    {
        translation += {{0,0,-1}};
    }
    if (state['d'])
    {
        translation += {{1,0,0}};
    }
    if (state[' '])
    {
        translation += {{0,1,0}};
    }
    if (state['c'])
    {
        translation += {{0,-1,0}};
    }

    for (auto c : movement_listeners)
    {
        c->translate(translation);
    }
}

void InputManager::register_movement_listener(const std::shared_ptr<Movable>& listener)
{
    movement_listeners.push_back(listener);
}

void InputManager::set_key(char c, bool state)
{
    this->state[c] = state;
}