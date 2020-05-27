//
// Created by mattrouss on 5/25/20.
//

#include "scene.h"

std::shared_ptr<Camera> Scene::init_camera(const YAML::Node &cam_node) const {
    auto z_near = cam_node["znear"] ? cam_node["znear"].as<float>() : 5;
    auto z_far = cam_node["zfar"] ? cam_node["zfar"].as<float>() : 2000;

    auto eye = cam_node["eye"] ? cam_node["eye"].as<mygl::Vec3>()
            : mygl::Vec3{{0, 0, 10}};
    auto target = cam_node["target"] ? cam_node["target"].as<mygl::Vec3>()
            : mygl::Vec3{{0, 0, 0}};
    auto up = cam_node["up"] ? cam_node["up"].as<mygl::Vec3>()
            : mygl::Vec3{{0, 1, 0}};

    auto cam = std::make_shared<Camera>(-1, 1, -1, 1, z_near, z_far);
    cam->look_at(eye, target, up);

    cam->set_prog_proj(prog_);
    cam->set_prog(prog_);
    inputManager.register_movement_listener(cam);

    return cam;
}

void Scene::init_mtls(const YAML::Node& mtls) {
    for (auto const& mtl: mtls) {
        std::cout << mtl << std::endl;
        mtls_.push_back(std::make_shared<Material>(
                texture_mngr_,
                prog_,
                mtl["texture_path"].as<std::string>(),
                mtl["normal_map_path"].as<std::string>()
                ));
    }
}

void Scene::init_objects(const YAML::Node &objs) {
    for (auto const& obj: objs) {
        auto mesh = mygl::load_mesh(obj["mesh_path"].as<std::string>());
        mesh->translate(obj["position"].as<mygl::Vec3>());
        renderers_.push_back(std::make_shared<ObjectRenderer>(
                prog_,
                mesh,
                mtls_[obj["material"].as<unsigned>()]
                ));
    }

}
