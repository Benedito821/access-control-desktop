cmake_minimum_required(VERSION 3.5)

project(access-control-admin VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql WebSockets HttpServer)

set(PROJECT_SOURCES
        main.cpp
        admin.cpp
        admin.h
        admin.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(access-control-admin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        defaultResources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET access-control-admin APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(access-control-admin SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(access-control-admin
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(access-control-admin PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql Qt6::WebSockets Qt6::HttpServer)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.access-control-admin)
endif()
set_target_properties(access-control-admin PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS access-control-admin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_program(CLANG_FORMAT clang-format)

file(GLOB_RECURSE ALL_SOURCE_FILES
    *.cpp *.h
)

add_custom_target(
    format
    COMMAND ${CLANG_FORMAT}
    -i
    ${ALL_SOURCE_FILES}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(access-control-admin)
endif()
