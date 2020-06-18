#include <iostream>

#include "waypoint_trajectory.h"

WaypointTrajectory::WaypointTrajectory(std::vector<Waypoint> waypoints, bool loop)
    : waypoint_list{waypoints}, loop{loop}, cursor{0}, start_time{0}
{
}

std::pair<mygl::Vec3, mygl::Vec3> WaypointTrajectory::operator()(float t)
{
    float delta_t = t - waypoint_list[cursor].time - start_time;
    //std::cout << "cursor: " << cursor << "\n";
    if (cursor < waypoint_list.size() - 1)
    {
        float t_dist = waypoint_list[cursor + 1].time - waypoint_list[cursor].time;
        float t_frac = delta_t / t_dist;

        auto res = (1 - t_frac) * waypoint_list[cursor].pos + t_frac * waypoint_list[cursor + 1].pos;

        if (t_frac >= 1.0 - 0.01)
        {
            cursor++;
        }

        //std::cout << "pos: " << res << "\n";
        return std::make_pair(res, mygl::Vec3{{0,0,0}});
    }
    else if (loop)
    {
        //set start_time
        //reset cursor to 0
        //teleport object to start
        start_time = t;
        cursor = 0;
        return std::make_pair(waypoint_list[cursor].pos, mygl::Vec3{{0,0,0}});
    }
    else
    {
        //do nothing, return last waypoint's position
        //std::cout << "pos: " << waypoint_list.back().pos << "\n";
        return std::make_pair(waypoint_list.back().pos, mygl::Vec3{{0,0,0}});
    }
}
