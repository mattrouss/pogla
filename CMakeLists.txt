cmake_minimum_required(VERSION 3.16)
project(bump)

set (BUMP_EXECUTABLE_NAME bump)
set (CROWD_SIM_EXECUTABLE_NAME crowd_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")


add_subdirectory(src)
add_subdirectory(third_party)

find_package(imgui CONFIG)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(YAML-CPP REQUIRED)

include_directories("third_party/imgui/")
link_directories("third_party/imgui/")

target_link_libraries(${BUMP_EXECUTABLE_NAME} PRIVATE -lGL -lGLEW -lglut -lpthread -lyaml-cpp)
target_link_libraries(${CROWD_SIM_EXECUTABLE_NAME} PRIVATE -lGL -lGLEW -lglut -lpthread -lyaml-cpp)
target_link_libraries(${CROWD_SIM_EXECUTABLE_NAME} PUBLIC imgui)

