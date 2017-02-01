
# Since std::thread is used C++11 is needed
add_definitions(-std=c++11)

add_library(simulator SHARED simulator.cpp)


