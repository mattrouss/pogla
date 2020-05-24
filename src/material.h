#ifndef BUMP_MATERIAL_H
#define BUMP_MATERIAL_H

#include <memory>
#include "texturemanager.h"
#include "program.h"

class Material
{
public:
    Material(std::shared_ptr<TextureManager> manager, mygl::program* prog,
            unsigned texture);
    Material(std::shared_ptr<TextureManager> manager, mygl::program* prog,
            unsigned texture, unsigned bump);
    Material(std::shared_ptr<TextureManager> manager, mygl::program* prog,
            std::string texture);
    Material(std::shared_ptr<TextureManager> manager, mygl::program* prog,
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
    mygl::program* program;
};


#endif //BUMP_MATERIAL_H
