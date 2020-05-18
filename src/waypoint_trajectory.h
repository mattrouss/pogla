#ifndef BUMP_WAYPOINT_TRAJECTORY_H
#define BUMP_WAYPOINT_TRAJECTORY_H


#include <utility>
#include "vector.h"

/*
 * This trajectory function only works in absolute time.
 */
class WaypointTrajectory
{
public:
    struct Waypoint
    {
        mygl::Vec3 pos;
        float time;
    };

    WaypointTrajectory(std::vector<Waypoint> waypoints, bool loop = false);


    std::pair<mygl::Vec3, mygl::Vec3> operator()(float t);

private:
    std::vector<Waypoint> waypoint_list;
    bool loop;
    size_t cursor;
    float start_time;
};


#endif //BUMP_WAYPOINT_TRAJECTORY_H
