project(Porthos)
 
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
 
SET ( PORTHOS_TITLE "Porthos" )
SET ( PORTHOS_SUBTITLE "An-embedded-linux-robot" )
SET ( PORTHOS_VERSION "0.0.0-cmake" )
OPTION(BUILD_PORTHOS_DOCUMENTATION "Create doxygen documentation for developers" ON)
OPTION(BUILD_PORTHOS_SIM_GUI "Generate GUI for simulation" OFF)

include_directories(inc)

set_property(GLOBAL PROPERTY C_STANDARD 99)

enable_testing()

add_subdirectory(deps)
add_subdirectory(lib)
add_subdirectory(doc)
