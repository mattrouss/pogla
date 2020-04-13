#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>

#include "program.h"
#include "matrix4.h"
#include "mesh.h"
#include "verts.h"
#include "camera.h"
#include "object_renderer.h"

std::vector<std::shared_ptr<ObjectRenderer>> renderers;

#define gl_err() \
{\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) std::cerr << "OpenGL error: " << __LINE__ << std::endl;\
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);gl_err();
    for (auto r : renderers)
    {
        r->render();
    }
    glutSwapBuffers();
}

bool initGlut(int &argc, char **argv) {
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Bump");
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
    glUniformMatrix4fv(mat_proj_id, 1, GL_FALSE, projection.transpose().data.data());gl_err();
    glUniformMatrix4fv(mat_obj_id, 1, GL_FALSE, model_view.transpose().data.data());gl_err();
}

int main(int argc, char **argv)
{
    initGlut(argc, argv);
    initGlew();
    init_gl();

    auto mesh = mygl::load_mesh("../meshes/monkey.obj");

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

    auto renderer = std::make_shared<ObjectRenderer>(prog, mesh);
    renderers.push_back(renderer);

    //init camera
    auto cam = Camera{-1, 1, -1, 1, 5, 2000};
    cam.look_at(10, 0, 10, 0, 0, 0, 0, 1, 0);
    auto projection_matrix = cam.get_projection_matrix();
    auto view_matrix = cam.get_view_matrix();
    std::cout << projection_matrix << "\n";
    std::cout << view_matrix << "\n";
    //configure uniforms and vbo
    init_uniforms(prog, {1, 1, 1, 1}, projection_matrix, view_matrix);

    glutMainLoop();
    return 0;
}
