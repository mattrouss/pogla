//
// Created by mattrouss on 5/25/20.
//

#ifndef BUMP_SCENE_H
#define BUMP_SCENE_H

#include <vector>
#include <yaml-cpp/yaml.h>
#include <map>
#include <memory>

#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "vector.h"
#include "material.h"
#include "inputmanager.h"
#include "object_renderer.h"
#include "trajectory_function.h"
#include "cameratracking.h"

class Scene {
public:
    Scene() {}

    void load_scene(const std::string& file_path, mygl::program* program);

    std::shared_ptr<Camera> init_camera(const YAML::Node& cam_node);
    void init_mtls(const YAML::Node& mtls);
    void init_objects(const YAML::Node& objs);
    void init_lights(const YAML::Node& lights);
    void init_trajectories(const YAML::Node& trajectories);

    void render() const;
    void run();

    std::shared_ptr<Light> get_light(size_t i) const;
    void set_lights_uniform() const;


private:
    mygl::program* prog_;

    std::shared_ptr<TextureManager> texture_mngr_;
    std::shared_ptr<LightManager> light_mngr_;

    std::shared_ptr<Camera> cam_;
    std::vector<std::shared_ptr<Material>> mtls_;
    std::vector<std::shared_ptr<ObjectRenderer>> renderers_;
    std::map<std::string, TrajectoryFunction> trajectory_functions_;
    std::vector<Trajectory> trajectories_;//trajectory instances for objects and lights
    std::shared_ptr<CameraTracking> tracking_;
};

#endif //BUMP_SCENE_H


