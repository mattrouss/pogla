#include "inputmanager.h"

InputManager inputManager;

void InputManager::send_keyboard_input()
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

    for (auto c : key_movement_listeners)
    {
        c->translate(translation);
    }
}

void InputManager::send_mouse_input()
{
    if (mouse_type == -1)
    {
        previous_mouse_coords = mouse_coords;
        return;
    }
    auto dp = drag_speed * (previous_mouse_coords - mouse_coords);
    mygl::Vec3 translation; 

    switch (mouse_type)
    {
        case GLUT_LEFT_BUTTON:
            translation = {{dp[0], dp[1], 0}};
            break;
        case GLUT_RIGHT_BUTTON:
            translation = {{0.0, 0.0, dp[1]}};
            break;
        default:
            break;
    }

    for (auto c : mouse_movement_listeners)
    {
        c->translate_local(translation);
    }
    previous_mouse_coords = mouse_coords;
    
}

void InputManager::register_keyboard_listener(const std::shared_ptr<Movable>& listener)
{
    key_movement_listeners.push_back(listener);
}

void InputManager::register_mouse_listener(const std::shared_ptr<Movable>& listener)
{
    mouse_movement_listeners.push_back(listener);
}

void InputManager::set_key(char c, bool state)
{
    this->state[c] = state;
}

void InputManager::set_mouse_type(int button, int state)
{
    if (state == GLUT_DOWN)
        mouse_type = button;
    else
        mouse_type = -1;
}

void InputManager::set_mouse_coords(int x, int y)
{
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    float ux = 2 * x / (float)w - 1;
    float uy = 1 - 2 * y / (float)h;

    mouse_coords = {{ux, uy}};

}

