cmake_minimum_required(VERSION 3.14)

project(RegressionAnalysis LANGUAGES CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/json
    ${CMAKE_SOURCE_DIR}/eigen
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

set(SRC_DATA_DIR ${CMAKE_SOURCE_DIR}/src/data)
set(SRC_CONFIGS_DIR ${CMAKE_SOURCE_DIR}/src/configs)
set(SRC_OUTPUTS_DIR ${CMAKE_SOURCE_DIR}/src/outputs)
set(SRC_PAINTINGS_DIR ${CMAKE_SOURCE_DIR}/src/PAINTINGS)

set(PYTHON_EXEC_PATH "/home/egor2320/MSnOutliers/.venv/bin/python3") # path to your python interpreter

configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)
include_directories(${CMAKE_BINARY_DIR})

file(WRITE ${CMAKE_SOURCE_DIR}/src/configs/config.py
"SRC_DATA_DIR = '${SRC_DATA_DIR}'\n"
"SRC_CONFIGS_DIR = '${SRC_CONFIGS_DIR}'\n"
"SRC_OUTPUTS_DIR = '${SRC_OUTPUTS_DIR}'\n"
"SRC_PAINTINGS_DIR = '${SRC_PAINTINGS_DIR}'\n"
"PYTHON_EXEC_PATH = '${PYTHON_EXEC_PATH}'\n"
)

set(LIBRARY_NAME RegressionAnalysisLib)
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

add_subdirectory(src)

add_subdirectory(ui)

# add_custom_target(clean_build COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/clean_build.cmake)

#enable_testing()
#add_subdirectory(tests)