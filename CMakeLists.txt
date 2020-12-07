cmake_minimum_required(VERSION 3.16)
project(bump)

set (BUMP_EXECUTABLE_NAME bump)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

add_subdirectory(src)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(YAML-CPP REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(${BUMP_EXECUTABLE_NAME} PRIVATE -lGL -lGLEW -lglut -lpthread -lyaml-cpp)
