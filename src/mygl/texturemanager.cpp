#include "texturemanager.h"
#include "mygl/gl_err.h"
#include "utils/image_io.h"

TextureManager::~TextureManager()
{
    glDeleteTextures(texture_ids.size(), texture_ids.data());
}

void TextureManager::use(unsigned i)
{
    if (i < texture_ids.size())
        glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
}

unsigned TextureManager::load(std::string path)
{
    GLuint texture_id;
    glGenTextures(1, &texture_id);gl_err()

    glBindTexture(GL_TEXTURE_2D, texture_id);gl_err()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);gl_err()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);gl_err()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);gl_err()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);gl_err()

    auto texture = io::load_image(path.c_str());
    if (!texture)
        return 0;//error

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, texture->pixels);
    gl_err()

    delete texture;

    glBindTexture(GL_TEXTURE_2D, 0);gl_err()//unbind current texture (return to neutral state)

    texture_ids.push_back(texture_id);
    return texture_ids.size() - 1;//return index of the texture
}

GLuint TextureManager::get(unsigned i) const
{
    if (i >= texture_ids.size())
        return 0;
    return texture_ids[i];
}

void TextureManager::reset_use()
{
    glBindTexture(GL_TEXTURE_2D, 0);gl_err()
}
