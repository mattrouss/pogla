cmake_minimum_required(VERSION 3.16)
project(bump)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

add_executable(bump src/main.cpp
        src/matrix4.h
        src/matrix4.cpp
        src/program.h
        src/program.cpp
        src/mesh.h
        src/mesh.cpp
        src/verts.h
        src/camera.cpp
        src/camera.h
        src/object_renderer.cpp
        src/object_renderer.h
        src/gl_err.h
        src/vector.h
        src/image.cpp
        src/image.h
        src/image_io.cpp
        src/image_io.h
        src/light.cpp src/light.h
        src/inputmanager.cpp
        src/inputmanager.h
        src/movable.h
        src/clock.cpp
        src/clock.h
        src/trajectory.cpp
        src/trajectory.h
        src/trajectory_function.h
        src/texturemanager.cpp
        src/texturemanager.h
        src/material.cpp
        src/material.h
        src/waypoint_trajectory.cpp
        src/waypoint_trajectory.h
        src/cameratracking.cpp
        src/cameratracking.h src/basicmovable.cpp src/basicmovable.h)

target_link_libraries(bump -lGL -lGLEW -lglut -lpthread)
