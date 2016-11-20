find_package(Check REQUIRED)

include_directories(${CHECK_INCLUDE_DIRS})

add_executable(test_libmotion test_libmotion.c)
target_link_libraries(test_libmotion motion ${CHECK_LIBRARIES})

add_executable(test_librange test_librange.c)
target_link_libraries(test_librange range ${CHECK_LIBRARIES})
