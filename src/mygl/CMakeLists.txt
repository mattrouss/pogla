add_subdirectory(assets)

set(mygl_src_files
    program.cpp program.h

    # Asset managers
    object_renderer.cpp object_renderer.h
    lightmanager.cpp lightmanager.h
    texturemanager.cpp texturemanager.h

    inputmanager.cpp inputmanager.h

    # OpenGL errors
    gl_err.h
)

add_library(mygl ${mygl_src_files})
target_link_libraries(mygl PUBLIC assets utils)

target_include_directories(assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

