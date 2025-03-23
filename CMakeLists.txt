cmake_minimum_required(VERSION 3.14)

project(RegressionAnalysis LANGUAGES CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB COMMON_SOURCES lib/COMMON/*.cpp)
file(GLOB DISTRIBUTION_SOURCES lib/DISTRIBUTIONS/*.cpp)
file(GLOB ML_SOURCES lib/ML/**/*.cpp)

set(SOURCES
    ${COMMON_SOURCES}
    ${DISTRIBUTION_SOURCES}
    ${ML_SOURCES}
)

set(LIBRARY_NAME RegressionAnalysisLib)
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)