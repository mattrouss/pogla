#ifndef BUMP_CAMERATRACKING_H
#define BUMP_CAMERATRACKING_H

#include <memory>
#include "trajectory/trajectory.h"
#include "camera.h"

class CameraTracking
{
public:

    CameraTracking(TrajectoryFunction cam_func, TrajectoryFunction obj_func,
            std::shared_ptr<Camera> cam);
    CameraTracking(TrajectoryFunction cam_func, std::shared_ptr<Camera> cam,
            mygl::Vec3 focus);
    CameraTracking(mygl::Vec3 pos, TrajectoryFunction obj_func,
            std::shared_ptr<Camera> cam);

    void run();//run for 1 frame

    std::function<void()> get_callback();//camera cannot track objects that need updating

private:
    std::shared_ptr<Trajectory> cam_trajectory;
    std::shared_ptr<Trajectory> obj_trajectory;

    std::shared_ptr<Camera> cam;
    std::shared_ptr<Movable> obj;
};


#endif //BUMP_CAMERATRACKING_H
