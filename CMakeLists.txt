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
file(GLOB MS_SOURCES lib/MS/*.cpp)
file(GLOB GD_SOURCES lib/GD/*.cpp)

set(SOURCES
    ${COMMON_SOURCES}
    ${DISTRIBUTION_SOURCES}
    ${ML_SOURCES}
    ${MS_SOURCES}
    ${GD_SOURCES}
)

set(DATA_DIR ${CMAKE_SOURCE_DIR}/src/data)
set(SRC_CONFIGS_DIR ${CMAKE_SOURCE_DIR}/src/configs)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)
include_directories(${CMAKE_BINARY_DIR})

file(WRITE ${CMAKE_SOURCE_DIR}/src/configs/config.py
"DATA_DIR = '${DATA_DIR}'\n"
"SRC_CONFIGS_DIR = '${SRC_CONFIGS_DIR}'\n"
)

set(LIBRARY_NAME RegressionAnalysisLib)
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)