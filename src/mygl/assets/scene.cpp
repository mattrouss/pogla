//
// Created by mattrouss on 5/25/20.
//

#include "scene.h"
#include "cameratracking.h"

void Scene::load_scene(const std::string &file_path, mygl::Program *program) {
    YAML::Node root = YAML::LoadFile(file_path);
    if (!root["scene"]) {
        throw std::invalid_argument(file_path + ": No scene was found.");
    }
    prog_ = program;
    auto sc = root["scene"];

    if (sc["trajectories"])
    {
        auto trajectories = sc["trajectories"];
        init_trajectories(trajectories);
    }

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

std::shared_ptr<Camera> Scene::init_camera(const YAML::Node &cam_node) {
    auto z_near = cam_node["znear"] ? cam_node["znear"].as<float>() : 5;
    auto z_far = cam_node["zfar"] ? cam_node["zfar"].as<float>() : 2000;

    auto eye = cam_node["eye"] ? cam_node["eye"].as<mygl::Vec3>()
            : mygl::Vec3{{0, 0, 10}};

    bool trajectory = false;
    std::string trajectory_name = "None";
    if (cam_node["trajectory"])
    {
        trajectory = true;
        trajectory_name = cam_node["trajectory"].as<std::string>();
    }

    auto up = cam_node["up"] ? cam_node["up"].as<mygl::Vec3>()
            : mygl::Vec3{{0, 1, 0}};

    auto cam = std::make_shared<Camera>(-1, 1, -1, 1, z_near, z_far);
    cam->set_prog(prog_);

    mygl::Vec3 target = mygl::Vec3{{0.001,0,0}};//not origin to prevent problems with initial
    //lookat call
    if (cam_node["target"])
    {
        if (cam_node["target"].IsScalar())
        {
            //set target to trajectory
            auto target_trajectory = cam_node["target"].as<std::string>();
            if (trajectory)
            {
                //trajectory with moving camera and target
                tracking_ = std::make_shared<CameraTracking>(trajectory_functions_.at(trajectory_name),
                                                             trajectory_functions_.at(target_trajectory),
                                                             cam);
            }
            else
            {
                //trajectory with only moving target
                tracking_ = std::make_shared<CameraTracking>(eye, trajectory_functions_.at(target_trajectory),
                                                             cam);
            }
        }
        else
        {
            target = cam_node["target"].as<mygl::Vec3>();
            if (trajectory)
            {
                //trajectory with moving camera and static target
                tracking_ = std::make_shared<CameraTracking>(trajectory_functions_.at(trajectory_name),
                                                             cam, target);
            }
            //else: no trajectory, camera is static
        }
    }

    cam->look_at(eye, target, up);

    cam->set_prog_proj(prog_);
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
        if (obj["rotation"])
            mesh->set_rot(obj["rotation"].as<mygl::Vec3>() / 180.0 * M_PI);

        if (obj["trajectory"])
        {
            trajectories_.emplace_back(trajectory_functions_[obj["trajectory"].as<std::string>()]);
            trajectories_.back().register_object(mesh);
        }

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

            if (light["trajectory"])
            {
                trajectories_.emplace_back(trajectory_functions_[light["trajectory"].as<std::string>()]);
                trajectories_.back().register_object(light_mngr_->get(idx));
            }

        }
        idx++;
    }
    light_mngr_->set_lights_uniform(prog_);
}

void Scene::init_trajectories(const YAML::Node& trajectories)
{
    for (auto const& trajectory: trajectories)
    {
        std::string type = "None";
        if (trajectory["type"])
            type = trajectory["type"].as<std::string>();
        else
            continue;//abort loading for current trajectory

        std::string name = trajectory["name"] ? trajectory["name"].as<std::string>() : "unnamed";
        if (name == "unnamed")
            std::cerr << "Warning: found unnamed trajectory.\n";
        if (type == "waypoints" and trajectory["data"] and trajectory["data"].IsSequence())
        {
            auto data = std::vector<WaypointTrajectory::Waypoint>{};
            for (auto const& wp : trajectory["data"])
            {
                data.push_back(WaypointTrajectory::Waypoint{wp.as<std::array<float, 4>>()});
            }
            auto result = TrajectoryFunction{WaypointTrajectory{data},
                    TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION};
            trajectory_functions_.emplace(name, result);
        }
        else if (type == "orbit")
        {
            auto origin = trajectory["origin"].as<mygl::Vec3>();
            auto radius = trajectory["radius"].as<float>();
            auto period = trajectory["period"].as<float>();
            auto result = TrajectoryFunction{
                    [origin, radius, period](float t) -> std::pair<mygl::Vec3, mygl::Vec3> {
                        float x = sinf(t/period*M_PI);
                        float y = 0;
                        float z = cosf(t/period*M_PI);

                        auto res = (mygl::Vec3{{x, y, z}} * radius) + origin;
                        return {res, {{0,0,0}}};
                    }, TFunc::ABS_POS|TFunc::ABS_TIME|TFunc::SET_POS|TFunc::USE_POSITION};
            trajectory_functions_.emplace(name, result);
        }
    }
}

void Scene::render() const {
    for (auto r : renderers_)
        r->render();
}

void Scene::run()
{
    if (tracking_ != nullptr)
        tracking_->run();

    for (auto& t : trajectories_)
    {
        t.run();
    }

    if (not trajectories_.empty())
        light_mngr_->set_lights_uniform(prog_);
}

std::shared_ptr<Light> Scene::get_light(size_t i) const {
    return light_mngr_->get(i);
}

void Scene::set_lights_uniform() const {
    light_mngr_->set_lights_uniform(prog_);
}
