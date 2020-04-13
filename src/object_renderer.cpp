#include "object_renderer.h"
#include "gl_err.h"

ObjectRenderer::ObjectRenderer(mygl::program *prog, std::shared_ptr<mygl::mesh> mesh)
{
    this->program = prog;
    this->mesh = mesh;

    glGenVertexArrays(1, &vao);gl_err()
    glBindVertexArray(vao);gl_err()

    GLuint vertex_buffer_id;
    glGenBuffers(1, &vertex_buffer_id);gl_err()

    auto verts = mesh->verts;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(mygl::Vertex), verts.data(), GL_STATIC_DRAW);gl_err();

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, pos)));gl_err();
    glEnableVertexAttribArray(0);gl_err();

    //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, normal)));gl_err();
    glEnableVertexAttribArray(1);gl_err();

    //uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, uv)));gl_err();
    glEnableVertexAttribArray(2);gl_err();

    //tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, tangent)));gl_err();
    glEnableVertexAttribArray(3);gl_err();



    buffer_ids.push_back(vertex_buffer_id);

    /*
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
     */

    glBindVertexArray(0);gl_err()
}

ObjectRenderer::~ObjectRenderer()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDeleteBuffers(buffer_ids.size(), (GLuint*)buffer_ids.data());gl_err()
    glDeleteVertexArrays(1, &vao);gl_err()
}

void ObjectRenderer::render() const
{
    glBindVertexArray(vao);gl_err()
    glDrawArrays(GL_TRIANGLES, 0, mesh->verts.size() * 3);gl_err()
    glBindVertexArray(0);gl_err()
}
