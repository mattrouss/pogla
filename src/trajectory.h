#ifndef BUMP_TRAJECTORY_H
#define BUMP_TRAJECTORY_H


#include <functional>
#include <memory>
#include "utils/vector.h"
#include "movable.h"
#include "utils/clock.h"
#include "trajectory_function.h"

class Trajectory
{
public:
    //translations set the type of movement that is used (translations or directly setting position)
    Trajectory(TrajectoryFunction func);
    void run();//run trajectory for 1 frame

    //do we need a function to run the trajectory in another thread?

    std::function<void()> get_callback();
    std::function<void()> get_callback_with_update(const std::function<void()>& update_func);

    void register_object(std::shared_ptr<Movable> obj);

private:
    TrajectoryFunction trajectory_function;
    std::vector<std::shared_ptr<Movable>> objects;
    mygl::Vec3 last_pos;
    mygl::Vec3 last_rot;
    Clock clock;
};


#endif //BUMP_TRAJECTORY_H
