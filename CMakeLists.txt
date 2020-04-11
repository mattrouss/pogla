cmake_minimum_required(VERSION 3.16)
project(pogl1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

add_executable(pogl1 src/main.cpp
    src/matrix4.h
    src/matrix4.cpp
    src/program.h
    src/program.cpp src/mesh.h src/mesh.cpp src/verts.h src/vec3.cpp src/vec3.h)

target_link_libraries(pogl1 -lGL -lGLEW -lglut -lpthread)
