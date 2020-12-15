#ifndef BUMP_OBJECT_RENDERER_H
#define BUMP_OBJECT_RENDERER_H


#include <GL/glew.h>
#include <vector>
#include "mygl/program/Program.h"
#include "assets/mesh.h"
#include "assets/material.h"

class ObjectRenderer
{
public:
    ObjectRenderer(mygl::Program* prog, std::shared_ptr<mygl::mesh> mesh, std::shared_ptr<Material> mat);
    ~ObjectRenderer();

    void render() const;

private:
    mygl::Program* program;
    std::shared_ptr<mygl::mesh> mesh;
    std::vector<GLint> buffer_ids;
    std::vector<GLuint> vertex_attribs;
    GLuint vao;
    std::shared_ptr<Material> mat;
};


#endif //BUMP_OBJECT_RENDERER_H
