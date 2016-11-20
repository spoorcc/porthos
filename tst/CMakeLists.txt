find_package(Check REQUIRED)

include_directories(${CHECK_INCLUDE_DIRS})

add_executable(test_libmotion test_libmotion.c)
target_link_libraries(test_libmotion motion ${CHECK_LIBRARIES})
