#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>
#include <mygl/program/programbuilder.h>
#include <mygl/shadertypes.h>

#include "assets/material.h"
#include "assets/scene.h"
#include "assets/cameratracking.h"

#include "mygl/basicmovable.h"
#include "mygl/gl_err.h"
#include "mygl/inputmanager.h"
#include "mygl/program/Program.h"
#include "trajectory/trajectory.h"
#include "utils/clock.h"
#include "utils/matrix4.h"

Scene scene;
std::function<void()> light_trajectory_callback;
std::function<void()> cam_trajectory_callback;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);gl_err();

    scene.render();

    glutSwapBuffers();
    inputManager.send_input();
    mainClock.tick();
}

void keyDown(unsigned char key, int, int)
{
    inputManager.set_key(key, true);
}

void keyUp(unsigned char key, int, int)
{
    inputManager.set_key(key, false);
}

void refresh_timer(int)
{
    glutPostRedisplay();
    //light_trajectory_callback();
    //cam_trajectory_callback();
    scene.run();
    glutTimerFunc(1000/50, refresh_timer, 0);
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
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
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

void init_color_uniform(mygl::Program* prog, std::array<float, 4> color)
{
    GLint color_id;
    color_id = glGetUniformLocation(prog->prog_id(), "color");gl_err();
    glUniform4f(color_id, color[0], color[1], color[2], color[3]);gl_err();
}

int main(int argc, char **argv)
{
    std::string scene_path;
    if (argc != 2)
    {
        std::cout << "usage: ./bump scene_file\n";
        return 1;
    }
    else
    {
        scene_path = std::string{argv[1]};
    }
    initGlut(argc, argv);
    initGlew();
    init_gl();

    std::string v_shader = "../shaders/vertex.shd";
    std::string f_shader = "../shaders/fragment.shd";
    //auto prog = mygl::Program::make_program(v_shader, f_shader);
    auto prog_builder = mygl::ProgramBuilder{};
    prog_builder.add_shader(
            mygl::shaders::DisplayShadersBase{v_shader, f_shader}
            );
    auto prog_result = prog_builder.build();
    if (prog_result == std::nullopt)
    {
        std::cout << "Shader compilation failed.\n";
        return 1;
    }
    auto prog = prog_result->get();
    prog->use();
    std::cout << "Hello, World!" << std::endl;

    scene.load_scene(scene_path, prog);

    //start display timer and start main loop
    glutTimerFunc(1000/50, refresh_timer, 0);
    glutMainLoop();
    return 0;
}
