#include <iostream>
#include "trajectory.h"

Trajectory::Trajectory(TrajectoryFunction func)
    : trajectory_function{func}
{
    clock.tick();
    auto tmp = trajectory_function(0.0f);
    last_pos = tmp.first;
    last_rot = tmp.second;
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
        auto pair = trajectory_function(t);
        auto translation = pair.first;
        auto rotation = pair.second;

        if (trajectory_function.translate())
        {
            if (trajectory_function.delta_pos())
            {
                if (trajectory_function.use_position())
                    obj->translate(translation);//we need to translate, function returns translation
                if (trajectory_function.use_rotation())
                    obj->rotate(rotation);
            }
            else
            {
                if (trajectory_function.use_position())
                {
                    auto delta_pos = translation - last_pos;
                    last_pos = translation;
                    obj->translate(delta_pos);//we want to translate, we have absolute position, convert to delta
                }
                if (trajectory_function.use_rotation())
                {
                    auto delta_rot = rotation - last_rot;
                    last_rot = rotation;
                    obj->rotate(delta_rot);
                }
            }
        }
        else
        {
            if (trajectory_function.delta_pos())
            {
                if (trajectory_function.use_position())
                {
                    last_pos += translation;
                    obj->set_pos(last_pos);//we have a delta, we want abolute pos, add v to last pos
                }
                if (trajectory_function.use_rotation())
                {
                    last_rot += rotation;
                    obj->set_rot(last_rot);
                }
            }
            else
            {
                if (trajectory_function.use_position())
                    obj->set_pos(translation);//we have abolute pos, we want to set position
                if (trajectory_function.use_rotation())
                    obj->set_rot(rotation);
            }
        }
        //std::cout << "Pos: " << obj->get_pos() << "\n";
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
