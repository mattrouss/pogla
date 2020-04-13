#include "object_renderer.h"
#include "gl_err.h"

ObjectRenderer::ObjectRenderer(mygl::program *prog, std::shared_ptr<mygl::mesh> mesh)
{
    this->program = prog;
    this->mesh = mesh;

    glGenVertexArrays(1, &vao);gl_err()
    glBindVertexArray(vao);gl_err()

    GLuint buffer_id[2];
    glGenBuffers(2, buffer_id);gl_err()

    //verts
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[0]);gl_err()
    glBufferData(GL_ARRAY_BUFFER, mesh->verts.size() * sizeof(float), mesh->verts.data(), GL_STATIC_DRAW);
    gl_err()
    glVertexAttribPointer(buffer_id[0], 3, GL_FLOAT, GL_FALSE, 0, 0);gl_err()
    glEnableVertexAttribArray(buffer_id[0]);gl_err()

    //normals
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[1]);gl_err()
    glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(float), mesh->normals.data(), GL_STATIC_DRAW);
    gl_err()
    glVertexAttribPointer(buffer_id[1], 3, GL_FLOAT, GL_FALSE, 0, 0);gl_err()
    glEnableVertexAttribArray(buffer_id[1]);gl_err()

    buffer_ids.push_back(buffer_id[0]);
    buffer_ids.push_back(buffer_id[1]);

    if (!mesh->uv.empty())
    {
        GLuint uv_id;
        glGenBuffers(1, &uv_id);gl_err()

        glBindBuffer(GL_ARRAY_BUFFER, uv_id);gl_err()
        glBufferData(GL_ARRAY_BUFFER, mesh->uv.size() * sizeof(float), mesh->uv.data(), GL_STATIC_DRAW);
        gl_err()
        glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);gl_err()
        glEnableVertexAttribArray(uv_id);gl_err()

        buffer_ids.push_back(uv_id);
    }

    glBindVertexArray(0);gl_err()
}

ObjectRenderer::~ObjectRenderer()
{
    glDeleteBuffers(buffer_ids.size(), (GLuint*)buffer_ids.data());gl_err()
    glDeleteVertexArrays(1, &vao);gl_err()
}

void ObjectRenderer::render() const
{
    glBindVertexArray(vao);gl_err()
    glDrawArrays(GL_TRIANGLES, 0, mesh->verts.size() * 3);gl_err()
    glBindVertexArray(0);gl_err()
}
