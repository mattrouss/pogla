add_subdirectory(utils)

add_executable(${BUMP_EXECUTABLE_NAME}
        main.cpp
        program.h
        program.cpp
        mesh.h
        mesh.cpp
        verts.h
        camera.cpp
        camera.h
        object_renderer.cpp
        object_renderer.h
        gl_err.h
        light.cpp
        light.h
        inputmanager.cpp
        inputmanager.h
        movable.h
        clock.cpp
        clock.h
        trajectory.cpp
        trajectory.h
        trajectory_function.h
        texturemanager.cpp
        texturemanager.h
        material.cpp
        material.h
        waypoint_trajectory.cpp
        waypoint_trajectory.h
        cameratracking.cpp
        cameratracking.h
        basicmovable.cpp
        basicmovable.h
        scene.cpp
        scene.h)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${BUMP_EXECUTABLE_NAME} PRIVATE utils)
