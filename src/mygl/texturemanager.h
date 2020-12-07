#ifndef BUMP_TEXTUREMANAGER_H
#define BUMP_TEXTUREMANAGER_H

#include <string>
#include <vector>
#include <GL/glew.h>

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager();

    void use(unsigned i);
    void reset_use();
    unsigned load(std::string path);
    GLuint get(unsigned i) const;

private:
    std::vector<GLuint> texture_ids;
};


#endif //BUMP_TEXTUREMANAGER_H
