//
// Created by mattrouss on 5/25/20.
//

#ifndef BUMP_SCENE_H
#define BUMP_SCENE_H

#include <vector>
#include <yaml-cpp/yaml.h>

#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "vector.h"
#include "material.h"
#include "inputmanager.h"
#include "object_renderer.h"

class Scene {
public:
    Scene() {}
    Scene(const std::string& filepath, mygl::program* program)
    : prog_(program)
    {
        YAML::Node root = YAML::LoadFile(filepath);
        if (!root["scene"]) {
            throw std::invalid_argument(filepath + ": No scene was found.");
        }
        auto sc = root["scene"];

        auto cam = sc["camera"];
        cam_ = init_camera(cam);

        texture_mngr_ = std::make_shared<TextureManager>();
        auto mtls = sc["materials"];
        init_mtls(mtls);

        auto objs = sc["objects"];
        init_objects(objs);
    }

    std::shared_ptr<Camera> init_camera(const YAML::Node& cam_node) const;
    void init_mtls(const YAML::Node& mtls);
    void init_objects(const YAML::Node& objs);

private:
    mygl::program* prog_;

    std::shared_ptr<TextureManager> texture_mngr_;
    std::vector<std::shared_ptr<ObjectRenderer>> renderers_;

    std::shared_ptr<Camera> cam_;
    std::vector<std::shared_ptr<Material>> mtls_;
    std::vector<Light> lights_;
};

#endif //BUMP_SCENE_H


