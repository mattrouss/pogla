#include "light.h"
#include "gl_err.h"

LightManager::LightManager()
{
    lights.fill(std::make_shared<Light>(mygl::Vec3{{{0,0,0}}},
            mygl::Vec3{{0,0,0}}, false));
}

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
UniformLight::UniformLight(mygl::Vec3 pos, mygl::Vec3 color, bool used)
    : Light(pos, color, LightType::UNIFORM, used)
{
}


DirectionalLight::DirectionalLight(mygl::Vec3 pos, mygl::Vec3 color, mygl::Vec3 target, bool used)
    : Light(pos, color, LightType::DIRECTIONAL, used), target(target)
{
}

mygl::Vec3 DirectionalLight::get_dir() {
    return (pos - target).normalized();
}


AmbientLight::AmbientLight(mygl::Vec3 pos, mygl::Vec3 color, bool used)
        : Light(pos, color, LightType::AMBIENT, used)
{
}

void LightManager::set_uniform(size_t i, mygl::Vec3 pos, mygl::Vec3 color)
{
    lights[i] = std::make_shared<UniformLight>(pos, color);
}


void LightManager::set_directional(size_t i, mygl::Vec3 pos, mygl::Vec3 target,
                                   mygl::Vec3 color) {
    lights[i] = std::make_shared<DirectionalLight>(pos, color, target);
}


void LightManager::set_ambient(size_t i, mygl::Vec3 pos, mygl::Vec3 color)
{
    lights[i] = std::make_shared<AmbientLight>(pos, color);
}

std::shared_ptr<Light> LightManager::get(size_t i)
{
    return lights[i];
}

void LightManager::set_lights_uniform(mygl::program *prog)
{
    GLint uniformId;
    uniformId = glGetUniformLocation(prog->prog_id(), "uniform_light");
    glUniform1ui(uniformId, LightType::UNIFORM);
    uniformId = glGetUniformLocation(prog->prog_id(), "directional_light");
    glUniform1ui(uniformId, LightType::DIRECTIONAL);
    uniformId = glGetUniformLocation(prog->prog_id(), "ambient_light");
    glUniform1ui(uniformId, LightType::AMBIENT);

    for (size_t i = 0; i < 10; i++)
    {
        //that's gonna be fun
        std::string prefix = "lights[" + std::to_string(i) + "].";
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
        if (lights[i]->get_type() == LightType::DIRECTIONAL) {
            uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "dir").c_str());gl_err()
            mygl::Vec3 dir = dynamic_cast<DirectionalLight&>(*lights[i]).get_dir();
            glUniform3f(uniformId,
                        dir[0],
                        dir[1],
                        dir[2]);
        }
        uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "type").c_str());gl_err()
        glUniform1i(uniformId, lights[i]->get_type());
        uniformId = glGetUniformLocation(prog->prog_id(), (prefix + "used").c_str());gl_err()
        glUniform1i(uniformId, lights[i]->used);
    }
}

void LightManager::reset(size_t i)
{
    lights[i] = std::make_shared<Light>(mygl::Vec3{{{0,0,0}}},
            mygl::Vec3{{0,0,0}}, false);
}