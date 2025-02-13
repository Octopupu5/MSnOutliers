cmake_minimum_required(VERSION 3.14)

project(RegressionAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
set(ML_DIR ${LIBRARY_DIR}/ML)

# main executable
add_executable(main_app ${SOURCE_DIR}/graph.cpp)

# own implementations
add_library(models STATIC
    ${ML_DIR}/DBSCAN/DBSCAN.cpp
    ${ML_DIR}/iForest/iForest.cpp
    ${ML_DIR}/iForest/Node.cpp
    ${ML_DIR}/GradientDescent/GD.cpp
    ${ML_DIR}/LinearRegression/LinearRegression.cpp
    ${ML_DIR}/OCSVM/OneClassSVM.cpp
)
target_include_directories(models PUBLIC ${ML_DIR})

# linking libraries to main executable
target_link_libraries(main_app PRIVATE models SDL2::SDL2)

# tests on
enable_testing()

# checking for packages
find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)
# find_package(Qt REQUIRED)

# adding tests
add_executable(run_tests
    ${TEST_DIR}/test_main.cpp
    ${TEST_DIR}/test_regression.cpp
    ${TEST_DIR}/test_metrics.cpp
)

# linking libraries to tests
target_link_libraries(run_tests PRIVATE models GTest::GTest GTest::Main)

add_test(NAME UnitTests COMMAND run_tests)

# for other OSs
install(TARGETS main_app run_tests models
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)