find_package(Check REQUIRED)

include_directories(${CHECK_INCLUDE_DIRS})

add_executable(test_libmotion test_libmotion.c)
target_link_libraries(test_libmotion motion ${CHECK_LIBRARIES})

add_executable(test_librange test_librange.c)
target_link_libraries(test_librange range ${CHECK_LIBRARIES})

add_executable(test_libmapper test_libmapper.c)
target_link_libraries(test_libmapper mapper ${CHECK_LIBRARIES})

add_executable(test_simulator test_simulator.c)
target_link_libraries(test_simulator simulator ${CHECK_LIBRARIES})

add_test (NAME libmotion COMMAND test_libmotion)
add_test (NAME librange  COMMAND test_librange)
add_test (NAME libmapper COMMAND test_libmapper)
add_test (NAME simulator COMMAND test_simulator)

