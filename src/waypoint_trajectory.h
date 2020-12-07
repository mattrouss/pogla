#ifndef BUMP_WAYPOINT_TRAJECTORY_H
#define BUMP_WAYPOINT_TRAJECTORY_H


#include <utility>
#include "utils/vector.h"

/*
 * This trajectory function only works in absolute time.
 */
class WaypointTrajectory
{
public:
    struct Waypoint
    {
        Waypoint() = default;
        Waypoint(std::array<float, 4> data)
        {
            time = data[0];
            pos = mygl::Vec3{{data[1], data[2], data[3]}};
        }

        float time;
        mygl::Vec3 pos;
    };

    WaypointTrajectory(std::vector<Waypoint> waypoints, bool loop = true);


    std::pair<mygl::Vec3, mygl::Vec3> operator()(float t);

private:
    std::vector<Waypoint> waypoint_list;
    bool loop;
    size_t cursor;
    float start_time;
};


#endif //BUMP_WAYPOINT_TRAJECTORY_H
