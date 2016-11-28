project(Porthos)
 
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
 
SET ( PORTHOS_TITLE "Porthos" )
SET ( PORTHOS_SUBTITLE "An-embedded-linux-robot" )
SET ( PORTHOS_VERSION "0.0.0-cmake" )
OPTION(BUILD_PORTHOS_DOCUMENTATION "Create doxygen documentation for developers" ON)

include_directories(inc)

enable_testing()

add_subdirectory(lib)
add_subdirectory(doc)
