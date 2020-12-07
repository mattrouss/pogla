#include "light.h"

Light::Light(mygl::Vec3 pos, mygl::Vec3 color, unsigned type, bool used)
    : pos{pos}, color{color}, type{type}, used{used}
{
}


void Light::set_pos(mygl::Vec3 p)
{
    pos = p;
}

void Light::translate(mygl::Vec3 v)
{
    pos += v;
}
UniformLight::UniformLight(mygl::Vec3 pos, mygl::Vec3 color, float intensity)
    : Light(pos, color, LightType::UNIFORM, true), intensity{intensity}
{
}

float UniformLight::get_intensity() const {
    return intensity;
}

DirectionalLight::DirectionalLight(mygl::Vec3 pos, mygl::Vec3 color, mygl::Vec3 target)
    : Light(pos, color, LightType::DIRECTIONAL, true), target(target)
{
}

mygl::Vec3 DirectionalLight::get_dir() const {
    return (pos - target).normalized();
}


AmbientLight::AmbientLight(mygl::Vec3 pos, mygl::Vec3 color)
        : Light(pos, color, LightType::AMBIENT, true)
{
}
