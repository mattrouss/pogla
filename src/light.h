#ifndef BUMP_LIGHT_H
#define BUMP_LIGHT_H

#include <memory>
#include "vector.h"
#include "matrix4.h"
#include "program.h"
#include "movable.h"

struct Light : public Movable
{
    Light() = default;
    Light(mygl::Vec3 pos, mygl::Vec3 color, bool used = false);

    mygl::Vec3 pos;
    mygl::Vec3 color;
    bool used = false;
    //add strength attribute

    void use()
    {
        used = true;
    }
    void translate(mygl::Vec3 v) override;
    void set_pos(mygl::Vec3 p) override;
    void rotate(mygl::Vec3) override {}
    void set_rot(mygl::Vec3) override {}
};

class LightManager
{
public:
    LightManager();

    void set(size_t i, mygl::Vec3 pos, mygl::Vec3 color);
    void reset(size_t i);
    std::shared_ptr<Light> get(size_t i);

    void set_lights_uniform(mygl::program* prog);

private:
    std::array<std::shared_ptr<Light>, 16> lights;
};

#endif //BUMP_LIGHT_H
