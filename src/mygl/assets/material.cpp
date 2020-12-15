#include "material.h"
#include "mygl/gl_err.h"

#include <utility>

Material::Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
        unsigned texture)
    : texture_manager{std::move(manager)}, texture_id{texture}, bump_id{0}, use_bump{false}
{
    program = prog;
}

Material::Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
        unsigned texture, unsigned bump)
    : texture_manager{std::move(manager)}, texture_id{texture}, bump_id{bump}, use_bump{true}
{
    program = prog;
}

Material::Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
        std::string texture)
    : texture_manager{std::move(manager)}
{
    texture_id = texture_manager->load(std::move(texture));
    bump_id = 0;
    use_bump = false;
    program = prog;
}

Material::Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
        std::string texture, std::string bump)
    : texture_manager{std::move(manager)}
{
    texture_id = texture_manager->load(std::move(texture));
    bump_id = texture_manager->load(std::move(bump));
    use_bump = true;
    program = prog;
}

void Material::set_texture(unsigned value)
{
    texture_id = value;
}

void Material::set_bump(unsigned value)
{
    bump_id = value;
    use_bump = value != 0;
}

void Material::use() const
{
    glActiveTexture(GL_TEXTURE0 + texture_manager->get(texture_id));gl_err()
    texture_manager->use(texture_id);

    glActiveTexture(GL_TEXTURE0 + texture_manager->get(bump_id));gl_err()
    texture_manager->use(bump_id);

    GLint enable_bump_id;
    enable_bump_id = glGetUniformLocation(program->prog_id(), "enableBumpMapping");gl_err();
    glUniform1i(enable_bump_id, (int)use_bump);gl_err();

    GLint tex_locs[2];
    tex_locs[0] = glGetUniformLocation(program->prog_id(), "tex_sampler");gl_err();
    glUniform1i(tex_locs[0], texture_manager->get(texture_id));gl_err();

    tex_locs[1] = glGetUniformLocation(program->prog_id(), "bump_map");gl_err();
    glUniform1i(tex_locs[1], texture_manager->get(bump_id));gl_err();
}

void Material::stop_use() const
{
    texture_manager->reset_use();
}
