aux_source_directory(. SRC_LIST)

add_executable(matrix_test ${SRC_LIST})

target_link_libraries(matrix_test PRIVATE ${GTEST_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(matrix_test PRIVATE ../include)

gtest_discover_tests(matrix_test)