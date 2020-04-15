#ifndef BUMP_LIGHT_H
#define BUMP_LIGHT_H

#include <memory>
#include "vector.h"
#include "matrix4.h"
#include "program.h"

struct Light
{
    mygl::Vec3 pos;
    mygl::Vec3 color;
    bool used = false;
    //add strength attribute

    void use()
    {
        used = true;
    }
    void translate(mygl::Vec3 v);
    void set_pos(mygl::Vec3 p);
};

class LightManager
{
public:
    LightManager();

    void set(size_t i, mygl::Vec3 pos, mygl::Vec3 color);
    void reset(size_t i);
    Light& get(size_t i);

    void set_lights_uniform(mygl::program* prog);

private:
    std::array<Light, 16> lights;
};

#endif //BUMP_LIGHT_H
