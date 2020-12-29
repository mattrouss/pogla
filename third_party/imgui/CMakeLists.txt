add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLEW)

set(
    imgui_src_files
    imconfig.h
    imgui.cpp imgui.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glut.cpp imgui_impl_glut.h
    imgui_impl_opengl3.cpp imgui_impl_opengl3.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    )

add_library(imgui ${imgui_src_files})
