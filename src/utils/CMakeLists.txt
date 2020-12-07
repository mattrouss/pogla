set(utils_src_files

    # Clock
    clock.cpp clock.h

    # Image IO
    image.cpp image.h
    image_io.cpp image_io.h

    # Data structures
    vector.h
    matrix4.cpp matrix4.h
)

add_library(utils ${utils_src_files})
