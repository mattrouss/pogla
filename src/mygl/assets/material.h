#ifndef BUMP_MATERIAL_H
#define BUMP_MATERIAL_H

#include <memory>
#include "mygl/texturemanager.h"
#include "mygl/program/Program.h"

class Material
{
public:
    Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
            unsigned texture);
    Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
            unsigned texture, unsigned bump);
    Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
            std::string texture);
    Material(std::shared_ptr<TextureManager> manager, mygl::Program* prog,
            std::string texture, std::string bump);

    void set_texture(unsigned value);
    void set_bump(unsigned value);

    void use() const;
    void stop_use() const;

private:
    std::shared_ptr<TextureManager> texture_manager;
    unsigned texture_id;
    unsigned  bump_id;
    bool use_bump;
    mygl::Program* program;
};


#endif //BUMP_MATERIAL_H
