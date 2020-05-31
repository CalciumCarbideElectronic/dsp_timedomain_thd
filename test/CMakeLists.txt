cmake_minimum_required(VERSION 3.1)
project(test_thd)

set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(GTest)

add_executable(test_thd 
    main.cpp
    ../src/thd.c
    test_phase_sync.cpp
    test_thd.cpp
    test_util.c
)

target_include_directories(
    test_thd
    PUBLIC ${GTEST_INCLUDE_DIRS}
    PUBLIC ${PROJECT_SOURCE_DIR}/../src/
)

target_link_libraries(
    test_thd
    ${GTEST_LIBRARIES}
    pthread
)

