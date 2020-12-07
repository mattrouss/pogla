#include "cameratracking.h"
#include "basicmovable.h"

#include <utility>
#include <functional>

CameraTracking::CameraTracking(TrajectoryFunction cam_func, TrajectoryFunction obj_func,
        std::shared_ptr<Camera> cam)
{
    this->cam = cam;
    this->obj = std::make_shared<BasicMovable>();

    cam_trajectory = std::make_shared<Trajectory>(cam_func);
    cam_trajectory->register_object(cam);

    obj_trajectory = std::make_shared<Trajectory>(obj_func);
    obj_trajectory->register_object(obj);
}

CameraTracking::CameraTracking(TrajectoryFunction cam_func, std::shared_ptr<Camera> cam,
        mygl::Vec3 focus)
{
    this->cam = cam;
    this->obj = std::make_shared<BasicMovable>(focus);

    cam_trajectory = std::make_shared<Trajectory>(cam_func);
    cam_trajectory->register_object(cam);

    obj_trajectory = nullptr;
}

CameraTracking::CameraTracking(mygl::Vec3 pos, TrajectoryFunction obj_func,
        std::shared_ptr<Camera> cam)
{
    this->cam = cam;
    this->obj = std::make_shared<BasicMovable>();

    obj_trajectory = std::make_shared<Trajectory>(obj_func);
    obj_trajectory->register_object(obj);

    cam_trajectory = nullptr;
}

void CameraTracking::run()
{
    if (cam_trajectory != nullptr)
        cam_trajectory->run();
    if (obj_trajectory != nullptr)
        obj_trajectory->run();

    //cam->lookat(obj)
    cam->look_at(cam->get_pos(), obj->get_pos(), mygl::Vec3{{0,1,0}});
    cam->update();
}

std::function<void()> CameraTracking::get_callback()
{
    return [this]()
    {
        this->run();
    };
}
