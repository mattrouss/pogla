#ifndef BUMP_GL_ERR_H
#define BUMP_GL_ERR_H

#include <iostream>

#define gl_err() \
{\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error l." << __LINE__ << ": " << err << std::endl;\
}

#endif //BUMP_GL_ERR_H
