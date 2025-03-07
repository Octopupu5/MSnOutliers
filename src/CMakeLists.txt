include_directories(${CMAKE_SOURCE_DIR}/lib/DISTRIBUTIONS)
include_directories(${CMAKE_SOURCE_DIR}/lib/ML)

add_executable(test_ndarray test_ndarray.cpp)

target_link_libraries(test_ndarray
    ErrorDistributions
    MLComponents
)