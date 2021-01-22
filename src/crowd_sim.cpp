#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>
#include <mygl/program/programbuilder.h>
#include <mygl/shadertypes.h>

#include "assets/material.h"
#include "assets/scene.h"
#include "mygl/assets/mesh.h"
#include "assets/cameratracking.h"

#include "mygl/particle_system.h"
#include "mygl/basicmovable.h"
#include "mygl/gl_err.h"
#include "mygl/inputmanager.h"
#include "mygl/program/Program.h"
#include "trajectory/trajectory.h"
#include "utils/clock.h"
#include "utils/matrix4.h"

mygl::ParticleSystem particle_system(1000u);
std::function<void()> light_trajectory_callback;
std::function<void()> cam_trajectory_callback;

void display()
{
    glClearColor(0.95f, 0.93f, 0.9f, 1.0f) ;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);gl_err();

    particle_system.render(mainClock.deltatime());

    glutSwapBuffers();
    inputManager.send_mouse_input();
    inputManager.send_keyboard_input();
    mainClock.tick();
}

void mouseClick(int button, int state, int, int)
{
    inputManager.set_mouse_type(button, state);
}

void mouseMove(int x, int y)
{
    inputManager.set_mouse_coords(x, y);
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
    glutTimerFunc(1000/50, refresh_timer, 0);
}

bool initGlut(int &argc, char **argv) {
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Crowd Simulation");
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
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
    if (argc != 1)
    {
        std::cout << "usage: ./crowd_sim\n";
        return 1;
    }

    initGlut(argc, argv);
    initGlew();
    init_gl();

    std::string v_shader = "../shaders/particle_vertex.shader";
    std::string f_shader = "../shaders/particle_fragment.shader";

    std::string c_shader = "../shaders/boids.shader";
    std::string sort_shader = "../shaders/spatial_sort.shader";

    auto prog_builder = mygl::ProgramBuilder{};
    prog_builder.add_shader(
            mygl::shaders::DisplayShadersBase{v_shader, f_shader}
            );
    auto compute_prog_builder = mygl::ProgramBuilder{};
    compute_prog_builder.add_shader(
            mygl::shaders::ComputeShader{c_shader}
            );
    auto sort_prog_builder = mygl::ProgramBuilder{};
    sort_prog_builder.add_shader(
            mygl::shaders::ComputeShader{sort_shader}
    );
    auto prog_result = prog_builder.build();
    auto compute_result = compute_prog_builder.build();
    auto sort_result = sort_prog_builder.build();
    if (prog_result == std::nullopt || compute_result == std::nullopt
        || sort_result == std::nullopt)
    {
        std::cout << "Shader compilation failed.\n";
        return 1;
    }
    auto prog = prog_result->get();
    auto compute_prog = compute_result->get();
    prog->use();

    //init camera
    auto cam = std::make_shared<Camera>(-1, 1, -1, 1, 5, 2000);
    cam->set_prog(prog);
    cam->look_at({{0, 0, 10}}, {{0, 0, 0}}, {{0, 1, 0}});

    cam->set_prog_proj(prog);
    inputManager.register_mouse_listener(cam);
    inputManager.register_keyboard_listener(cam);

    auto lights = LightManager{};
    lights.set_directional(0, {{5,5,5}}, {{0, 0, 0}}, {{1,1,0.8}});
    lights.set_ambient(1, {{0,0,0}}, 0.2 * mygl::Vec3({1,1,1}));
    lights.set_lights_uniform(prog);

    init_color_uniform(prog, {{0.6, 0.8, 0.9, 1}});

    // Load particle mesh
    auto mesh = mygl::load_mesh("../meshes/fish.obj");

    // Load texture
    auto material = Material(std::make_shared<TextureManager>(), prog, "../textures/fish_uv.tga");
    material.use();

    particle_system.init_system(prog, compute_prog, sort_result->get(),  mesh);

    //start display timer and start main loop
    glutTimerFunc(1000/50, refresh_timer, 0);
    glutMainLoop();
    return 0;
}

