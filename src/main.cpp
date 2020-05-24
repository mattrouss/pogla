#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <array>

#include "program.h"
#include "image_io.h"
#include "matrix4.h"
#include "mesh.h"
#include "camera.h"
#include "object_renderer.h"
#include "gl_err.h"
#include "light.h"
#include "inputmanager.h"
#include "clock.h"
#include "trajectory.h"

std::vector<std::shared_ptr<ObjectRenderer>> renderers;
std::function<void()> light_trajectory_callback;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);gl_err();
    for (auto r : renderers)
    {
        r->render();
    }
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
    light_trajectory_callback();
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

void init_samplers(mygl::program* prog, bool enableBumpMapping) {
   GLuint texture_ids[2];
   glGenTextures(2, texture_ids);gl_err();

   glActiveTexture(GL_TEXTURE0 + texture_ids[0]);
   glBindTexture(GL_TEXTURE_2D, texture_ids[0]);gl_err();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   auto brick_tex = io::load_image("../textures/pebble_texture.tga");
   if (!brick_tex)
       return;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brick_tex->width, brick_tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, brick_tex->pixels);gl_err();
   delete brick_tex;

   glActiveTexture(GL_TEXTURE0 + texture_ids[1]);
   glBindTexture(GL_TEXTURE_2D, texture_ids[1]);gl_err();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   auto normal_map = io::load_image("../textures/pebble_normal.tga");
   if (!normal_map)
       return;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normal_map->width, normal_map->height, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_map->pixels);gl_err();
   delete normal_map;

    GLint enable_bump_id;
    enable_bump_id = glGetUniformLocation(prog->prog_id(), "enableBumpMapping");gl_err();
    glUniform1i(enable_bump_id, (int)enableBumpMapping);gl_err();

    GLint tex_locs[2];
    tex_locs[0] = glGetUniformLocation(prog->prog_id(), "tex_sampler");gl_err();
    glUniform1i(tex_locs[0], texture_ids[0]);gl_err();

    tex_locs[1] = glGetUniformLocation(prog->prog_id(), "bump_map");gl_err();
    glUniform1i(tex_locs[1], texture_ids[1]);gl_err();
}

void init_color_uniform(mygl::program* prog, std::array<float, 4> color)
{
    GLint color_id;
    color_id = glGetUniformLocation(prog->prog_id(), "color");gl_err();
    glUniform4f(color_id, color[0], color[1], color[2], color[3]);gl_err();
}

int main(int argc, char **argv)
{
    initGlut(argc, argv);
    initGlew();
    init_gl();

    auto mesh = mygl::load_mesh("../meshes/cube.obj");

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
    bool enableBumpMapping = false;
    auto cam = std::make_shared<Camera>(-1, 1, -1, 1, 5, 2000);
    cam->look_at({{0, 0, 10}}, {{0, 0, 0}}, {{0, 1, 0}});
  
    //configure samplers, uniforms and vbo
    init_color_uniform(prog, {1, 1, 1, 1});
    init_samplers(prog, enableBumpMapping);

    cam->set_prog_proj(prog);
    cam->set_prog(prog);
    inputManager.register_movement_listener(cam);

    //configure lights
    auto lights = LightManager{};
    lights.set_uniform(0, {{0,0,2}},{{1, 1, 0.8}},15);
    lights.set_ambient(1, {{0,0,0}},0.2 * mygl::Vec3({1,1,1}));
    lights.set_lights_uniform(prog);

    //set light trajectory
    auto light_movement = Trajectory{{[](float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float x = sinf(t);
        float z = cosf(t*3.0f);
        float y = cosf(t / 10.0f);

        auto res = mygl::Vec3{{0, 0, z}} * 2.0f;
        return {mygl::Vec3{{0,0,5}} + res, {{0,0,0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION}};
    light_movement.register_object(lights.get(0));

    /*auto light_movement = Trajectory{{[] (float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
        float rot = t;
        float y = sinf(t);

        auto res = mygl::Vec3{{0.0f, y, 0.0f}} * 2.0f;
        return {res, {{0, t, 0}}};
    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION|TFunc::USE_ROTATION}};
    light_movement.register_object(mesh);*/

    //temporary until a better solution is found
    light_trajectory_callback = light_movement.get_callback_with_update(
            [&]()
            {
                lights.set_lights_uniform(prog);
            }
            );
    //light_trajectory_callback = light_movement.get_callback();

    //start display timer and start main loop
    glutTimerFunc(1000/50, refresh_timer, 0);
    glutMainLoop();
    return 0;
}
