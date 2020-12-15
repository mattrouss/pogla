#include "object_renderer.h"

#include <utility>
#include "gl_err.h"

ObjectRenderer::ObjectRenderer(mygl::Program *prog, std::shared_ptr<mygl::mesh> mesh,
                               std::shared_ptr<Material> mat)
{
    this->program = prog;
    this->mesh = mesh;
    this->vertex_attribs = {0, 1, 2, 3};

    glGenVertexArrays(1, &vao);gl_err()
    glBindVertexArray(vao);gl_err()

    GLuint vertex_buffer_id;
    glGenBuffers(1, &vertex_buffer_id);gl_err()

    auto verts = mesh->verts;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);gl_err();
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(mygl::Vertex), verts.data(), GL_STATIC_DRAW);gl_err();

    //positions
    glVertexAttribPointer(vertex_attribs[0], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, pos)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[0]);gl_err();

    //normals
    glVertexAttribPointer(vertex_attribs[1], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, normal)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[1]);gl_err();

    //uv
    glVertexAttribPointer(vertex_attribs[2], 2, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, uv)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[2]);gl_err();

    //tangent
    glVertexAttribPointer(vertex_attribs[3], 3, GL_FLOAT, GL_FALSE, sizeof(mygl::Vertex), (void *)(offsetof(mygl::Vertex, tangent)));gl_err();
    glEnableVertexAttribArray(vertex_attribs[3]);gl_err();

    buffer_ids.push_back(vertex_buffer_id);

    glBindVertexArray(0);gl_err()

    this->mat = std::move(mat);
}

ObjectRenderer::~ObjectRenderer()
{
    for (GLuint vertex_attrib : vertex_attribs)
        glDisableVertexAttribArray(vertex_attrib);
    glDeleteBuffers(buffer_ids.size(), (GLuint*)buffer_ids.data());gl_err()
    glDeleteVertexArrays(1, &vao);gl_err()
}

void ObjectRenderer::render() const
{
    mat->use();
    GLint transform_id;
    transform_id = glGetUniformLocation(program->prog_id(), "model_matrix");gl_err()
    glUniformMatrix4fv(transform_id, 1, GL_FALSE, mesh->get_transform().transpose().data.data());gl_err()
    glBindVertexArray(vao);gl_err()
    glDrawArrays(GL_TRIANGLES, 0, mesh->verts.size() * 3);gl_err()
    glBindVertexArray(0);gl_err()
    mat->stop_use();
}
