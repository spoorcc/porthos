
# Since std::thread is used C++11 is needed
add_definitions(-std=c++11)

include_directories(inc)

IF( BUILD_PORTHOS_SIM_GUI)
    add_definitions(-DPORTHOS_WITH_GUI)
ENDIF( BUILD_PORTHOS_SIM_GUI)

add_library(simulator SHARED simulator.cpp)
add_library(collision_engine SHARED src/collision_engine.cpp)
add_library(factory SHARED src/factory.cpp)
add_library(move_engine SHARED src/move_engine.cpp)
add_library(ray_trace_engine SHARED src/ray_trace_engine.cpp)
target_link_libraries(simulator move_engine collision_engine factory ray_trace_engine)

IF( BUILD_PORTHOS_SIM_GUI)
    add_library(render_engine SHARED src/render_engine.cpp)
    target_include_directories(render_engine PRIVATE ${glfw_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS})
    target_link_libraries(render_engine glfw glad  ${OPENGL_gl_LIBRARY})
    target_link_libraries(simulator render_engine)

    add_executable(test_gl tst/test_gl.c)
    target_link_libraries(test_gl glfw glad  ${OPENGL_gl_LIBRARY})
ENDIF( BUILD_PORTHOS_SIM_GUI)

find_package(Check REQUIRED)
add_executable(test_simulator tst/test_simulator.c)
target_link_libraries(test_simulator simulator ${CHECK_LIBRARIES})

add_test (NAME simulator COMMAND test_simulator)
