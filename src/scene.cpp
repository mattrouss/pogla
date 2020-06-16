//
// Created by mattrouss on 5/25/20.
//

#include "scene.h"

void Scene::load_scene(const std::string &file_path, mygl::program *program) {
    YAML::Node root = YAML::LoadFile(file_path);
    if (!root["scene"]) {
        throw std::invalid_argument(file_path + ": No scene was found.");
    }
    prog_ = program;
    auto sc = root["scene"];

    // Setup camera
    auto cam = sc["camera"];
    cam_ = init_camera(cam);

    // Load materials
    texture_mngr_ = std::make_shared<TextureManager>();
    auto mtls = sc["materials"];
    init_mtls(mtls);

    // Load object meshes
    auto objs = sc["objects"];
    init_objects(objs);

    // configure lights
    auto lights = sc["lights"];
    init_lights(lights);
}

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

void Scene::init_lights(const YAML::Node &lights) {
    auto idx = 0u;
    light_mngr_ = std::make_shared<LightManager>();
    for (auto const& light: lights) {
        auto type = light["type"] ? light["type"].as<std::string>() : "ambient";
        if (type == "ambient") {
            light_mngr_->set_ambient(idx,
                    light["position"].as<mygl::Vec3>(),
                    light["color"].as<mygl::Vec3>());
        } else if (type == "uniform") {
            light_mngr_->set_uniform(idx,
                    light["position"].as<mygl::Vec3>(),
                    light["color"].as<mygl::Vec3>(),
                    light["intensity"].as<float>());

        }
        idx++;
    }
    light_mngr_->set_lights_uniform(prog_);
}

void Scene::render() const {
    for (auto r : renderers_)
        r->render();
}

std::shared_ptr<Light> Scene::get_light(size_t i) const {
    return light_mngr_->get(i);
}

void Scene::set_lights_uniform() const {
    light_mngr_->set_lights_uniform(prog_);
}
