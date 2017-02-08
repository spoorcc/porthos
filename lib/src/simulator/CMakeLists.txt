
# Since std::thread is used C++11 is needed
add_definitions(-std=c++11)

IF( PORTHOS_BUILD_SIM_GUI)
    add_definitions(-DPORTHOS_WITH_GUI)
ENDIF( PORTHOS_BUILD_SIM_GUI)

add_library(simulator SHARED simulator.cpp)

IF( PORTHOS_BUILD_SIM_GUI)
    add_library(render_engine SHARED render_engine.cpp)
    target_include_directories(simulator PRIVATE ${glfw_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS})
    target_link_libraries(simulator glfw glad  ${OPENGL_gl_LIBRARY})

ENDIF( PORTHOS_BUILD_SIM_GUI)
