#ifndef BUMP_GL_ERR_H
#define BUMP_GL_ERR_H

#define gl_err() \
{\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;\
}

#endif //BUMP_GL_ERR_H
