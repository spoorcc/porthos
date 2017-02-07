
# Since std::thread is used C++11 is needed
add_definitions(-std=c++11)

add_library(simulator SHARED simulator.cpp)

target_include_directories(simulator PRIVATE ${glfw_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS})

target_link_libraries(simulator glfw glad  ${OPENGL_gl_LIBRARY})

