cmake_minimum_required(VERSION 3.14)

# project name
project(RegressionAnalysis LANGUAGES CXX)

# cpp standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# constants
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

# main executable
# add_executable(main_app ${SOURCE_DIR}/main.cpp)
add_executable(main_app ${SOURCE_DIR}/graph.cpp)

# own implementations
add_library(regression STATIC 
    ${LIBRARY_DIR}/RegressionModel.cpp
    ${LIBRARY_DIR}/Metrics.cpp
)
target_include_directories(regression PUBLIC ${LIBRARY_DIR})

# linking libraries to main executable
target_link_libraries(main_app PRIVATE regression SDL2::SDL2)

# tests on
enable_testing()

# checking for packages
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)

# adding tests
add_executable(run_tests
    ${TEST_DIR}/test_main.cpp
    ${TEST_DIR}/test_regression.cpp
    ${TEST_DIR}/test_metrics.cpp
)

# linking libraries to tests
target_link_libraries(run_tests PRIVATE regression GTest::GTest GTest::Main)

add_test(NAME UnitTests COMMAND run_tests)

# for other OSs
install(TARGETS main_app run_tests regression
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)