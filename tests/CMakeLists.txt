include_directories(${CMAKE_SOURCE_DIR}/lib/DISTRIBUTIONS)
include_directories(${CMAKE_SOURCE_DIR}/lib/ML)

add_executable(test_main test_main.cpp)
add_executable(test_metrics test_metrics.cpp)
add_executable(test_regression test_regression.cpp)

target_link_libraries(test_main
    ErrorDistributions
    MLComponents
)

target_link_libraries(test_metrics
    ErrorDistributions
    MLComponents
)

target_link_libraries(test_regression
    ErrorDistributions
    MLComponents
)