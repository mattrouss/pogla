#include "trajectory.h"

Trajectory::Trajectory(TrajectoryFunction func)
    : trajectory_function{func}
{
    clock.tick();
    last_pos = trajectory_function(0.0f);
}

void Trajectory::run()
{
    float t = clock.deltatime(true);//a bit more expensive but better precision
    // and works even with only one tick
    if (trajectory_function.delta_time())
    {
        clock.tick();
    }
    for (auto obj : objects)
    {
        auto v = trajectory_function(t);

        if (trajectory_function.translate())
        {
            if (trajectory_function.delta_pos())
                obj->translate(v);//we need to translate, function returns translation
            else
            {
                auto delta = v - last_pos;
                last_pos = v;
                obj->translate(delta);//we want to translate, we have absolute position, convert to delta
            }
        }
        else
        {
            if (trajectory_function.delta_pos())
            {
                last_pos += v;
                obj->set_pos(last_pos);//we have a delta, we want abolute pos, add v to last pos
            }
            else
            {
                obj->set_pos(v);//we have abolute pos, we want to set position
            }
        }
    }
}

void Trajectory::register_object(std::shared_ptr<Movable> obj)
{
    objects.push_back(obj);
}

std::function<void()> Trajectory::get_callback()
{
    return [this]()
    {
        this->run();
    };
}

std::function<void()> Trajectory::get_callback_with_update(const std::function<void()>& update_func)
{
    return [this, update_func]()
    {
        this->run();
        update_func();
    };
}
