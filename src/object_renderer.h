#ifndef BUMP_OBJECT_RENDERER_H
#define BUMP_OBJECT_RENDERER_H


#include <GL/glew.h>
#include <vector>
#include "program.h"
#include "mesh.h"
#include "material.h"

class ObjectRenderer
{
public:
    ObjectRenderer(mygl::program* prog, std::shared_ptr<mygl::mesh> mesh, std::shared_ptr<Material> mat);
    ~ObjectRenderer();

    void render() const;

private:
    mygl::program* program;
    std::shared_ptr<mygl::mesh> mesh;
    std::vector<GLint> buffer_ids;
    std::vector<GLuint> vertex_attribs;
    GLuint vao;
    std::shared_ptr<Material> mat;
};


#endif //BUMP_OBJECT_RENDERER_H
