add_subdirectory(mygl)
add_subdirectory(utils)

add_executable(${BUMP_EXECUTABLE_NAME}
        bump.cpp
)

add_executable(${CROWD_SIM_EXECUTABLE_NAME}
        crowd_sim.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

target_link_libraries(${BUMP_EXECUTABLE_NAME} PUBLIC mygl utils)
target_link_libraries(${CROWD_SIM_EXECUTABLE_NAME} PUBLIC mygl utils)

target_include_directories(mygl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
