include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DISTRIBUTIONS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ML)

add_library(ErrorDistributions
    DISTRIBUTIONS/ErrorDistributions.cpp
)

add_library(MLComponents
    Ndarray.cpp
    DataProcessor.cpp
)