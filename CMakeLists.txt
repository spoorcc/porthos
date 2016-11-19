project(Porthos)
 
cmake_minimum_required(VERSION 2.8)
 
SET ( PORTHOS_TITLE "Porthos" )
SET ( PORTHOS_SUBTITLE "An-embedded-linux-robot" )
SET ( PORTHOS_VERSION "0.0.0-cmake" )
OPTION(BUILD_PORTHOS_DOCUMENTATION "Create doxygen documentation for developers" ON)

#add_subdirectory(src)
add_subdirectory(doc)

