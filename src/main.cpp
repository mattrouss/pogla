#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>

#include "program.h"
#include "matrix4.h"
#include "vector.h"
#include "mesh.h"
#include "verts.h"
#include "camera.h"

GLuint vao;

#define gl_err() \
{\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;\
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);gl_err();
    glBindVertexArray(vao);gl_err();
    glDrawArrays(GL_TRIANGLES, 0, verts.size() * 3);gl_err();
    glBindVertexArray(0);gl_err();
    glutSwapBuffers();
}

bool initGlut(int &argc, char **argv) {
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Test OpenGL - POGL");
    glutDisplayFunc(display);
    return true;//thanks, now I can really test for errors
}

bool initGlew() {
    return (glewInit() == GLEW_OK);
}

bool init_gl()
{
    glEnable(GL_DEPTH_TEST);gl_err();
    glDepthFunc(GL_LESS);gl_err();
    glDepthRange(0.0, 1.0);gl_err();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);gl_err();
    glEnable(GL_CULL_FACE);gl_err();
    glCullFace(GL_FRONT);gl_err();
    glFrontFace(GL_CCW);gl_err();
    glClearColor(0.0, 0.0, 0.0, 1);gl_err();

    return true;
}

void init_uniforms(mygl::program* prog, std::array<float, 4> color, mygl::matrix4 projection,
        mygl::matrix4 model_view)
{
    GLint color_id;
    GLint mat_proj_id;
    GLint mat_obj_id;
    color_id = glGetUniformLocation(prog->prog_id(), "color");gl_err();
    glUniform4f(color_id, color[0], color[1], color[2], color[3]);gl_err();

    mat_proj_id = glGetUniformLocation(prog->prog_id(), "projection_matrix");gl_err();
    mat_obj_id = glGetUniformLocation(prog->prog_id(), "model_view_matrix");gl_err();
    glUniformMatrix4fv(mat_proj_id, 1, GL_FALSE, projection.data.data());gl_err();
    glUniformMatrix4fv(mat_obj_id, 1, GL_FALSE, model_view.data.data());gl_err();
}

GLuint initVBO(mygl::program* prog)
{
    GLint obj_location = glGetAttribLocation(prog->prog_id(), "position");gl_err();

    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);gl_err();
    glBindVertexArray(vao_id);gl_err();

    GLuint buffer_id[2];
    glGenBuffers(2, buffer_id);gl_err();//works because we only ask for one buffer


    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[0]);gl_err();
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);gl_err();
    glVertexAttribPointer(buffer_id[0], 3, GL_FLOAT, GL_FALSE, 0, 0);gl_err();
    glEnableVertexAttribArray(buffer_id[0]);gl_err();

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id[1]);gl_err();
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);gl_err();
    glVertexAttribPointer(buffer_id[1], 3, GL_FLOAT, GL_FALSE, 0, 0);gl_err();
    glEnableVertexAttribArray(buffer_id[1]);gl_err();

    return vao_id;
}

int main(int argc, char **argv)
{
    initGlut(argc, argv);
    initGlew();
    init_gl();

    auto mesh = mygl::load_mesh("../meshes/monkey.obj");
    verts = mesh->verts;
    normals = mesh->normals;

    std::string v_shader = "../shaders/vertex.shd";
    std::string f_shader = "../shaders/fragment.shd";
    auto prog = mygl::program::make_program(v_shader, f_shader);
    if (prog == nullptr)
    {
        std::cout << "Shader compilation failed.\n";
        return 1;
    }
    prog->use();
    std::cout << "Hello, World!" << std::endl;

    //init camera
    auto cam = Camera{-1, 1, -1, 1, 5, 2000};
    cam.look_at(0, 0, 10, 0, 0, 0, 0, 1, 0);
    auto projection_matrix = cam.get_projection_matrix();
    auto view_matrix = cam.get_view_matrix();
    std::cout << projection_matrix << "\n";
    std::cout << view_matrix << "\n";
    //configure uniforms and vbo
    init_uniforms(prog, {1, 1, 1, 1}, projection_matrix, view_matrix);
    vao = initVBO(prog);

    glutMainLoop();

    glDeleteVertexArrays(1, &vao);gl_err();
    return 0;
}
