#ifndef BUMP_INPUTMANAGER_H
#define BUMP_INPUTMANAGER_H

#include <GL/freeglut.h>

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
        mouse_type = -1;
    }
    void register_keyboard_listener(const std::shared_ptr<Movable>& listener);
    void register_mouse_listener(const std::shared_ptr<Movable>& listener);
    void send_keyboard_input();
    void send_mouse_input();
    void set_key(char c, bool state);
    void set_mouse_type(int button, int state);
    void set_mouse_coords(int x, int y);

private:
    std::vector<std::shared_ptr<Movable>> key_movement_listeners;
    std::vector<std::shared_ptr<Movable>> mouse_movement_listeners;
    std::array<bool, 256> state;

    float drag_speed = 1000.0f;
    int mouse_type;
    mygl::Vec2 mouse_coords;
    mygl::Vec2 previous_mouse_coords;
};

extern InputManager inputManager;

#endif //BUMP_INPUTMANAGER_H
