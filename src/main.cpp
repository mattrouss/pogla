#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>

#include "program.h"
#include "utils/matrix4.h"
#include "gl_err.h"
#include "inputmanager.h"
#include "clock.h"
#include "trajectory.h"
#include "material.h"
#include "cameratracking.h"
#include "basicmovable.h"
#include "scene.h"

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

void init_color_uniform(mygl::program* prog, std::array<float, 4> color)
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
        std::cout << "usage: ./bump scane_file\n";
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
    auto prog = mygl::program::make_program(v_shader, f_shader);
    if (prog == nullptr)
    {
        std::cout << "Shader compilation failed.\n";
        return 1;
    }
    prog->use();
    std::cout << "Hello, World!" << std::endl;

    scene.load_scene(scene_path, prog);

    //init camera
    bool enableBumpMapping = false;
    //auto cam = std::make_shared<Camera>(-1, 1, -1, 1, 5, 2000);
    //cam->set_prog(prog);
    //cam->look_at({{0, 0, 10}}, {{0, 0, 0}}, {{0, 1, 0}});
  
    //configure samplers, uniforms and vbo
    /*init_color_uniform(prog, {1, 1, 1, 1});

    //cam->set_prog_proj(prog);
    //inputManager.register_movement_listener(cam);

    //configure lights
    auto lights = LightManager{};
    lights.set_directional(0, {{5,5,5}}, {{0, 0, 0}}, {{1,1,0.8}});
    lights.set_ambient(1, {{0,0,0}}, 0.2 * mygl::Vec3({1,1,1}));
    lights.set_lights_uniform(prog);

    //set light trajectory
    auto light_movement = Trajectory{{[](float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float x = sinf(t);
        float z = cosf(t*3.0f);
        float y = cosf(t / 10.0f);

        auto res = mygl::Vec3{{0, 0, z}} * 2.0f;
        return {mygl::Vec3{{0,0,5}} + res, {{0,0,0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION}};
    //light_movement.register_object(lights.get(0));
    //light_movement.register_object(cam);
    light_movement.register_object(scene.get_light(0));

    auto cam_movement = TrajectoryFunction{[](float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float x = sinf(t/10.f);
        float y = 5;
        float z = cosf(t/10.f);

        auto res = mygl::Vec3{{x, y, z}} * 5.f;
        return {res, {{0,0,0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION};

    auto light_func = TrajectoryFunction{[](float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float x = sinf(t);
        float z = cosf(t);
        float y = cosf(t / 10.0f);

        auto res = mygl::Vec3{{x, y, z}} * 3.0f;
        return {res, {{0,0,0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION};
    auto cam_target = std::make_shared<BasicMovable>();
    //auto tracking = CameraTracking(cam_movement, light_func, cam, cam_target);

    /*auto light_movement = Trajectory{{[] (float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float rot = t;
        float y = sinf(t);

        auto res = mygl::Vec3{{0.0f, y, 0.0f}} * 2.0f;
        return {res, {{0, t, 0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION|TFunc::USE_ROTATION}};
    light_movement.register_object(mesh);*/

    //temporary until a better solution is found
    /*light_trajectory_callback = light_movement.get_callback_with_update(
            [&]()
            {
                //lights.set_lights_uniform(prog);
                //cam->set_prog_proj(prog);
                scene.set_lights_uniform();
            }
            );*/
    //light_trajectory_callback = light_movement.get_callback();

    //cam_trajectory_callback = tracking.get_callback();


    //start display timer and start main loop
    glutTimerFunc(1000/50, refresh_timer, 0);
    glutMainLoop();
    return 0;
}
