#include "cameratracking.h"

#include <utility>
#include <functional>

CameraTracking::CameraTracking(TrajectoryFunction cam_func, TrajectoryFunction obj_func,
        std::shared_ptr<Camera> cam, std::shared_ptr<Movable> obj)
{
    this->cam = cam;
    this->obj = obj;

    cam_trajectory = std::make_shared<Trajectory>(cam_func);
    cam_trajectory->register_object(cam);

    obj_trajectory = std::make_shared<Trajectory>(obj_func);
    obj_trajectory->register_object(obj);
}

void CameraTracking::run()
{
    cam_trajectory->run();
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
