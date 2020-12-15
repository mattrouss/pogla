#ifndef BUMP_LIGHTMANAGER_H
#define BUMP_LIGHTMANAGER_H

#include <memory>

#include "assets/light.h"
#include "utils/vector.h"
#include "utils/matrix4.h"
#include "mygl/program/Program.h"
#include "movable.h"

class LightManager
{
public:
    LightManager();

    void set_uniform(size_t i, mygl::Vec3 pos, mygl::Vec3 color, float intensity);
    void set_directional(size_t i, mygl::Vec3 pos, mygl::Vec3 target, mygl::Vec3 color);
    void set_ambient(size_t i, mygl::Vec3 pos, mygl::Vec3 color);
    void reset(size_t i);
    std::shared_ptr<Light> get(size_t i);

    void set_lights_uniform(mygl::Program* prog);

private:
    std::array<std::shared_ptr<Light>, 16> lights;
};

#endif //BUMP_LIGHTMANAGER_H
