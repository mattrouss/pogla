set(program_src_files
    Program.cpp Program.h
    programbuilder.cpp programbuilder.h programbuilder.hpp
)

add_library(program ${program_src_files})
target_link_libraries(program PRIVATE mygl)
