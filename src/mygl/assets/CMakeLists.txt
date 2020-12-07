set(assets_src_files
    light.cpp light.h
    material.cpp material.h
    mesh.cpp mesh.h
    scene.cpp scene.h
    verts.h

    # Camera
    camera.cpp camera.h
    cameratracking.cpp cameratracking.h
)

add_library(assets ${assets_src_files})
target_link_libraries(assets PRIVATE mygl)
