cmake_minimum_required(VERSION 3.16)

project(ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PATH_TO_OUTPUT ${CMAKE_SOURCE_DIR}/)
set(PATH_TO_BINARY ${CMAKE_SOURCE_DIR}/build/src/main)
set(PATH_TO_PICTURES ${CMAKE_SOURCE_DIR}/src/outputs/)

configure_file(
    ${CMAKE_SOURCE_DIR}/ui/config.h.in
    ${CMAKE_BINARY_DIR}/ui/config.h
)
include_directories(${CMAKE_BINARY_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        modeldialog.cpp
        modeldialog.h
        generatedialog.cpp
        generatedialog.h
        modeltemplate.cpp
        modeltemplate.h
        basicstatdialog.cpp
        basicstatdialog.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        modeldialog.h modeldialog.cpp
    )
else()
    if(ANDROID)
        add_library(ui SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ui)
endif()
set_target_properties(ui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ui)
endif()
