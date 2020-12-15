add_subdirectory(assets)
add_subdirectory(trajectory)
add_subdirectory(program)

set(mygl_src_files
    #program compilation
    shadertypes.h

    # Asset managers
    object_renderer.cpp object_renderer.h
    lightmanager.cpp lightmanager.h
    texturemanager.cpp texturemanager.h

    inputmanager.cpp inputmanager.h

    # Movable
    movable.h
    basicmovable.cpp basicmovable.h

    # OpenGL errors
    gl_err.h
)

add_library(mygl ${mygl_src_files})
target_link_libraries(mygl PUBLIC assets trajectory utils program)

target_include_directories(assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(trajectory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(program PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

