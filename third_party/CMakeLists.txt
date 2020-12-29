add_subdirectory(imgui)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CROWD_SIM_EXECUTABLE_NAME} PUBLIC imgui)
