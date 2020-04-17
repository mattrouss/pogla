#include "light.h"
#include "gl_err.h"

LightManager::LightManager()
{
    lights.fill(std::make_shared<Light>(mygl::Vec3{{{0,0,0}}},
            mygl::Vec3{{0,0,0}}, false));
}

Light::Light(mygl::Vec3 pos, mygl::Vec3 color, bool used)
    : pos{pos}, color{color}, used{used}
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

void LightManager::set(size_t i, mygl::Vec3 pos, mygl::Vec3 color)
{
    lights[i] = std::make_shared<Light>(pos, color);
}

std::shared_ptr<Light> LightManager::get(size_t i)
{
    return lights[i];
}

void LightManager::set_lights_uniform(mygl::program *prog)
{
    for (size_t i = 0; i < 16; i++)
    {
        //that's gonna be fun
        std::string prefix = "lights[" + std::to_string(i) + "].";
        GLint uniformId;
        uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "position").c_str());gl_err()
        glUniform3f(uniformId,
                lights[i]->pos[0],
                lights[i]->pos[1],
                lights[i]->pos[2]);
        uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "color").c_str());gl_err()
        glUniform3f(uniformId,
                lights[i]->color[0],
                lights[i]->color[1],
                lights[i]->color[2]);
        uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "used").c_str());gl_err()
        glUniform1i(uniformId, lights[i]->used);
    }
}

void LightManager::reset(size_t i)
{
    lights[i] = std::make_shared<Light>(mygl::Vec3{{{0,0,0}}},
            mygl::Vec3{{0,0,0}}, false);
}
