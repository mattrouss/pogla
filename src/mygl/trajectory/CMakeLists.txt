set(trajectory_src_files
    trajectory.cpp trajectory.h
    trajectory_function.h

    waypoint_trajectory.cpp waypoint_trajectory.h
)

add_library(trajectory ${trajectory_src_files})
target_link_libraries(trajectory PRIVATE mygl)
